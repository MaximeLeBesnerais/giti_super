from pydub import AudioSegment
from pydub.playback import play
import os


def giti_vahan():
    script_dir = os.path.dirname(__file__)
    # song is in assets folder
    song = os.path.join(script_dir, 'assets', 'birthday.m4a')
    song = AudioSegment.from_file(song, format="m4a")
    play(song)
