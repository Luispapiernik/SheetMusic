OBJECTS = frequency.o audio.o miscellaneous.o
FLAGS = -lm -lsndfile -lfftw3
CC = gcc

$(OBJECTS): frequency.h audio.h
parser.o: miscellaneous.h parser.h
audio_test.o: audio.h
frequency_test.o: audio.h frequency.h miscellaneous.h
parser_test.o: parser.h


.PHONY: makeAudioTest
makeAudioTest: $(OBJECTS) audio_test.o
	gcc -o audio_test audio_test.o $(OBJECTS) $(FLAGS)

.PHONY: cleanAudioTest
cleanAudioTest:
	rm audio_test.o

.PHONY: cleanAudioExec
cleanAudioExec:
	rm audio_test


.PHONY: makeFrequencyTest
makeFrequencyTest: $(OBJECTS) frequency_test.o
	gcc -o frequency_test frequency_test.o $(OBJECTS) $(FLAGS)

.PHONY: cleanFrequencyTest
cleanFrequencyTest:
	rm frequency_test.o

.PHONY: cleanFrequencyExec
cleanFrequencyExec:
	rm frequency_test


.PHONY: makeParserTest
makeParserTest: miscellaneous.o parser.o parser_test.o
	gcc -o parser_test miscellaneous.o parser.o parser_test.o $(FLAGS)

.PHONY: cleanParserTest
cleanParserTest:
	rm parser_test.o

.PHONY: cleanParserExec
cleanParserExec:
	rm parser_test


.PHONY: cleanExec
cleanExec:
	rm audio_test frequency_test parser_test sheetMusic

.PHONY: clean
clean:
	rm *.o
