#!/usr/bin/python
# -*- encoding: utf - 8 -*-

from __future__ import division

from argparse import ArgumentParser

from scipy.io import wavfile
import numpy as np
import math


def frequency(note, eighth):
    """
    Esta funcion retorna la frecuencia de una nota dada en una octava dada.
    La indexacion de las notas es: C -> 1, C# -> 2, D -> 3,... B -> 12.
    """
    return 440 * math.exp((eighth - 4 + (note - 10) / 12) * math.log(2))


def generateTone(amplitude, frequency, time, framerate):
    """
    Esta funcion recibe una frecuencia un tiempo en segundos y una tasa de
    muestreo, y retorna una señal con la frecuencia dada
    """
    t = np.linspace(0, time, framerate * time)
    data = amplitude * np.sin(2 * np.pi * frequency * t)

    return data


def generateAudioFile(filename, amplitude, frequencies, times, framerate, dt):
    """
    Esta funcion genera un archivo de audio a partir de unas frecuencias,
    tiempos y una tasa de muestreo
    """
    data = []

    for i in range(len(frequencies)):
        if dt:
            data.extend([0] * int(dt * framerate))
        data.extend(generateTone(amplitude, frequencies[i], times[i],
                                 framerate))

    wavfile.write(filename, framerate, np.array(data))


def extend(iterable, length):
    """iterable es una lista"""
    iterable.extend([iterable[-1]] * length)


def main():
    parser = ArgumentParser()

    parser.add_argument('filename')
    parser.add_argument('-a', '--amplitude', type=float, default=5)
    parser.add_argument('-f', '--framerate', type=int, default=44100)
    parser.add_argument('-t', '--times', type=float, default=[1], nargs='*')
    parser.add_argument('-frq', '--frequencies', type=float, default=[],
                        nargs='*')
    parser.add_argument('-n', '--notes', type=int, default=[1], nargs='*',
                        choices=range(1, 14))
    parser.add_argument('-e', '--eighths', type=int, default=[1], nargs='*')
    parser.add_argument('-dt', '--time-separation', type=float, default=0)

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

    generateAudioFile(args.filename, args.amplitude, frequencies, args.times,
                      args.framerate, args.time_separation)

    print('File generated: ')
    print('\tFrequencies: %s' % frequencies)
    print('\tTime: %s' % args.times)


if __name__ == '__main__':
    main()
