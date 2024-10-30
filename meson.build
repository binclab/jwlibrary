
project('jwlibrary', 'c')
webkitdep = dependency('webkitgtk-6.0')
gstreamerdep = dependency('gstreamer-1.0')
gstvideodep = dependency('gstreamer-video-1.0')
sqlitedep = dependency('sqlite3')
libzipdep = dependency('libzip')
subdir('res')
subdir('gui')
subdir('reader')
executable('jwlibrary', 'main.c', gui_sources, reader_sources, 
    dependencies : [webkitdep, gstreamerdep, gstvideodep, sqlitedep, libzipdep]
)