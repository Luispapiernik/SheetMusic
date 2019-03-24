from argparse import ArgumentParser

from scipy.io import wavfile
import numpy as np
import math


def frequency(note, eighth):
    return 440 * math.exp((eighth - 4 + (note - 10) / 12) * math.log(2))


def generateTone(frequency, time, framerate):
    """tiempo en segundos"""
    t = np.linspace(0, time, framerate * time)
    data = 5 * np.sin(2 * np.pi * frequency * t)

    return data


def generateAudioFile(filename, frequencies, times, framerate):
    data = []

    for i in range(len(frequencies)):
        data.extend(generateTone(frequencies[i], times[i], framerate))

    wavfile.write(filename, framerate, np.array(data))


def extend(iterable, length):
    """iterable es una lista"""
    iterable.extend([iterable[-1]] * length)


def main():
    parser = ArgumentParser()

    parser.add_argument('filename')
    parser.add_argument('-f', '--framerate', type=int, default=44100)
    parser.add_argument('-t', '--times', type=float, default=[1], nargs='*')
    parser.add_argument('-frq', '--frequencies', type=float, default=[],
                        nargs='*')
    parser.add_argument('-n', '--notes', type=int, default=[1], nargs='*',
                        choices=range(1, 14))
    parser.add_argument('-e', '--eighths', type=int, default=[1], nargs='*')

    args = parser.parse_args()

    diff = len(args.notes) - len(args.eighths)
    if diff > 0:
        extend(args.eighths, diff)
    if diff < 0:
        extend(args.notes, abs(diff))

    frequencies = [frequency(args.notes[i], args.eighths[i])
                   for i in range(len(args.notes))]

    frequencies = args.frequencies or frequencies

    diff = len(frequencies) - len(args.times)
    if diff > 0:
        extend(args.times, diff)

    generateAudioFile(args.filename, frequencies, args.times, args.framerate)

    print('File generated: ')
    print('\tFrequencies: %s' % frequencies)
    print('\tTime: %s' % args.times)


if __name__ == '__main__':
    main()
