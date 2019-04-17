CC = gcc
CFLAGS = -Wall -lm -lsndfile -lfftw3
DEPENDENCIES = miscellaneous.h audio.h frequency.h parser.h
OBJECTS = miscellaneous.o audio.o frequency.o parser.o

################################################################
# compilacion del programa principal
sheetMusic: sheetMusic.o
	$(CC) $(OBJECTS) sheetMusic.o -o sheetMusic $(CFLAGS)

sheetMusic.o: $(DEPENDENCIES) $(OBJECTS)
################################################################

################################################################
# para testear la funcionalidad del modulo audio.h audio.c
makeAudioTest: audio.o audio_test.o
	$(CC) audio.o audio_test.o -o audio_test $(CFLAGS)

audio.o: miscellaneous.h audio.h
audio_test.o: audio.h

cleanAudioTest:
	rm miscellaneous.o
	rm audio.o
	rm audio_test.o
################################################################

################################################################
# para testear la funcionalidad en el modulo frequency.h frequency.c
makeFrequencyTest: miscellaneous.o audio.o frequency.o frequency_test.o
	$(CC) miscellaneous.o audio.o frequency.o frequency_test.o -o frequency_test $(CFLAGS)

frequency.o: miscellaneous.h audio.h miscellaneous.o audio.o
frequency_test.o: miscellaneous.h audio.h frequency.h miscellaneous.o audio.o frequency.o

cleanFrequencyTest:
	rm miscellaneous.o
	rm audio.o
	rm frequency.o
	rm frequency_test.o
################################################################

################################################################
# para testear la funcionalidad en el modulo parser.h parser.c
makeParserTest: miscellaneous.o parser.o parser_test.o
	$(CC) miscellaneous.o parser.o parser_test.o -o parser_test $(CFLAGS)

parser.o: miscellaneous.h parser.h miscellaneous.o
parser_test.o: parser.h parser.o

cleanParserTest:
	rm miscellaneous.o
	rm parser.o
	rm parser_test.o
################################################################


################################################################
# general rules
cleanObjects:
	rm *.o

cleanExec:
	rm audio_test
	rm frequency_test
	rm parser_test

cleanAll:
	rm *.o
	rm audio_test
	rm frequency_test
	rm parser_test 
################################################################