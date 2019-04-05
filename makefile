OBJECTS = frequency.o audio.o
FLAGS = -lm -lsndfile -lfftw3

sheetMusic:

$(OBJECTS): frequency.h audio.h
audio_test.o: audio.h
frequency_test.o: audio.h frequency.h


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


.PHONY: cleanExec
cleanExec:
	rm audio_test frequency_test sheetMusic

.PHONY: clean
clean:
	rm $(OBJECTS) audio_test.o frequency_test.o
