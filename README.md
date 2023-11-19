# Rc-Media-Player
Remote control Media Player, libmpv and Qt based.

Very simple QtQuick OpenGL container for libmpv. Provides basic controls, better than the OSD of mpv itself, with selector for video and audio tracks as well as selecting or adding a subtitle or remove an external subtitle already loaded. - The player starts in hidden mode and will show the UI when a controller client connects to the WebSocket and starts playback the UI will show. The WebSocket will accept JSON objects with various commands, any number of commands can be specified in the same object.

Commands supported:

**play**: string | object
  If string, will try to load the file or connect to the path based on the Url type specifier. file:// is prefered for loading files, but not nessecary.
    dvd:// should be used for iso images, no dvd navigation is availablem it will play the longest available title.
  If object, the object must contain the key "movie" that specifies the media file to play and can contain any of the following keys that will be set from the start of playback:
    videoTrack, audioTrack, subtitleTrack, position, volume, playlist. See the following for descriptions. If playback is not already happening, the keys by themselves will not have any effect, so if any of them are desired for playback from the beginning they should be specified in the play object. After the first position object have been recieved the commands can be used to modify playback.
**videoTrack**: integer, specify which video track to select for playback when multiple tracks are available.
**audioTrack**: integer, specify which audio track to select for playback when multiple tracks are available.
**subtitleTrack**: integer | string, if string, loads specified file as subtitle, if integer, select specified embedded subtitle track.
**position**: integer, move the playback position to the specified amount of seconds from the beginning.
**volume**: integer, sets the output volume. 0 is silent, 100 is zero db gain.
**playlist**: boolean, set the playlist controls visible in the UI if true or hides them if false.
**pause**: boolean, pauses or resumes playback. Value of true pauses, value of false resumes.
**stop**: (ignored), stops current playback and hides the UI.

The player will write the following json objects back to the client:

**audioTrack**: integer, the audio track being played.
**videoTrack**: integer, the video track being played.
**subtitleTrack**: integer | string, the subtite track being plated.
**subtitleRemoved**: string, when a subtitle file is being removed by the user from the UI.
**position**: integer, the number of seconds that have currently been played.
**volume**: integer, from 0 to 100 when the user changes volume from the UI.
*stopped*: string, when the playback have been stopped, the string being a reason or file ended, error, cancelled, next or previous.
*error*: string, any error is reported back.

  
