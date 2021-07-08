from mingus.containers import Note, Bar, Track
from mingus.midi import midi_file_out, fluidsynth
import sys
import argparse

def create_parser ():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--play', nargs='+', default=False)
    parser.add_argument('-m', '--midi', nargs='+', default=False)
    return parser

def create_midi(row):
    trk = Track()
    for note in row:
        trk.add_notes(note)
    midi_file_out.write_Track("row.mid", trk, 65)

def play_row(row):
    trk = Track()
    fluidsynth.init("samples.sf2", "alsa")
    for note in row:
        trk.add_notes(note)
    fluidsynth.play_Track(trk, 1, 65)

def main():
    parser = create_parser()
    namespace = parser.parse_args()


    if (namespace.play != False):
        play_row(namespace.play)
    elif (namespace.midi != False):
        create_midi(namespace.midi)

main()