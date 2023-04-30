
project('jwlibrary', 'c')
webkitdep = dependency('webkitgtk-6.0')
gstreamerdep = dependency('gstreamer-1.0')
gstvideodep = dependency('gstreamer-video-1.0')
subdir('gui')
executable('jwlibrary', 'main.c', gui_sources, dependencies : [webkitdep, gstreamerdep, gstvideodep])