class Translator

  # Word class
  class Word
    def initialize(name, pos)
      @name = name
      @pos = pos
      @translations = Hash.new
    end

    def getName
      @name
    end

    def getPos
      @pos
    end

    def getWordTranslation(lang)
      if lang == "English"
        return @name
      end
      
      if @translations.key?(lang)
        return @translations[lang]
      else
        nil
      end
    end

    def addTranslation(lang, word)
      @translations[lang] = word
    end
  end
  
  def initialize(words_file, grammar_file)
    @words = Array.new
    @langGrammar = Hash.new
    # language => grammar format (array)
    
    updateLexicon(words_file)
    updateGrammar(grammar_file)
  end

  def getLanguageWords(lang)
    langWords = Hash.new
    
    for i in @words
      if lang == "English"
        name = i.getName
      else
        name = i.getWordTranslation(lang)
      end
      
      if name != nil
        if langWords[i.getPos] == nil
          langWords[i.getPos] = Array.new.push(name)
        else
          langWords[i.getPos].push(name)
        end
      end
    end

    if langWords.empty?
      nil
    else
      langWords
    end
  end

  # part 1
  
  def updateLexicon(inputfile)
    f = File.open(inputfile)
    regex = /^([-a-z]+), ([A-Z]{3}), (([A-Z][a-z0-9]*):([-a-z]+)(, )?)*$/

    line = f.gets
    while line
      # read line
      if line =~ regex
        if getWord($1, $2, "English") == nil
          # name & POS
          word = Word.new($1, $2)
        else
          word = getWord($1, $2, "English")
          @words.delete(word)
        end
        # make array for languages & translations
        languages = line.scan(/([A-Z][a-z0-9]*):([-a-z]+)/)
        for i in languages
          word.addTranslation(i[0], i[1])
        end

        @words.push(word)
      end
      line = f.gets
    end
  end
  
  def updateGrammar(inputfile)
    f = File.open(inputfile)
    regex = /^([A-Z][a-z0-9]*): ([A-Z]{3}(\{[1-9]\d*\})?(, )?)*$/

    line = f.gets
    while line
      if line =~ regex
        name = $1
        # array of grammar order
        order = Array.new
        # array of grammar (not extracted)
        posx = line.scan(/([A-Z]{3})({([1-9]\d*)})?/)

        for i in posx
          count = i[2]
          if count == nil
            order.push(i[0])
          else
            count = count.to_i
            for j in 0..count - 1
              order.push(i[0])
            end
          end
        end
        @langGrammar[name] = order
      end
      line = f.gets
    end
  end

  # part 2
  
  def generateSentence(language, struct)
    sentence = ""
    # Hash that contains POS => [arr of words] in the correct language
    wordList = getLanguageWords(language)
        
    if wordList != nil
      gram = checkStruct(struct)
      if gram == nil
        return nil
      end
      
      for i in gram
        currWord = wordList[i]
        if currWord != nil
          currWord = currWord[0]
          sentence.concat(currWord + " ")
        else
          return nil
        end
      end
    end
    # puts "Sentence: " + sentence

    if sentence.empty?
      nil
    else
      sentence.delete_suffix!(" ")
      sentence
    end
  end
  
  def checkGrammar(sentence, language)
    # get language grammar
    gram = @langGrammar[language]
    if gram == nil
      return false
    end
    # array of words from sentence
    sen = sentence.split(" ")

    for i in 0..gram.length - 1
      curr = sen[i]
      pos = gram[i]

      wordPos = getWord(curr, pos, language) # Word
      if wordPos != nil
        wordPos = wordPos.getPos
      end
      
      # puts "comparing: " + wordPos + pos
      if wordPos == nil || wordPos != pos
        return false
      end                                              
    end
    true
  end

  # find Word
  def getWord(word, pos, language)
    for i in @words
      w = i.getWordTranslation(language)
      if pos == "NA" && w != nil && w == word
        return i
      end
      if w != nil && w == word && i.getPos == pos
        return i
      end
    end
    nil
  end

  def checkStruct(struct)
    if struct.is_a?(String)
      if @langGrammar[struct] != nil
        @langGrammar[struct].clone
      else
        nil
      end
    else
      struct
    end
  end
        
  def changeGrammar(sentence, struct1, struct2)
    newSentence = ""
    # array of words
    sen = sentence.split(" ")

    oldStruct = checkStruct(struct1)
    newStruct = checkStruct(struct2)
    if oldStruct == nil || newStruct == nil
      return nil
    end

    for i in newStruct
      idx = oldStruct.find_index(i)

      if idx != nil && sen[idx] != nil
        newSentence.concat(sen[idx] + " ")
        oldStruct[idx] = nil
       else
         return nil
      end
    end
    newSentence.delete_suffix!(" ")
    newSentence
  end

  # part 3
  
  def changeLanguage(sentence, language1, language2)
    newSentence = ""
    sen = sentence.split(" ")

    for i in sen
      currWord = getWord(i, "NA", language1)
      if currWord != nil
        oldWord = currWord.getWordTranslation(language1)
        newWord = currWord.getWordTranslation(language2)
        
        if oldWord != nil && newWord != nil
          newSentence.concat(newWord + " ")
        else
          return nil
        end
      else
        return nil
      end
    end

    newSentence.delete_suffix!(" ")
    newSentence
  end
  
  def translate(sentence, language1, language2)
    newSentence = changeGrammar(sentence, language1, language2)

    if newSentence != nil
      newSentence = changeLanguage(newSentence, language1, language2)
      if newSentence != nil
        return newSentence
      end
    end
  end
  nil
end  
