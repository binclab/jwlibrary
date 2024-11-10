project('jwlibrary', 'c')
webkitdep = dependency('webkitgtk-6.0')
gstreamerdep = dependency('gstreamer-1.0')
gstvideodep = dependency('gstreamer-video-1.0')
sqlitedep = dependency('sqlite3')
libzipdep = dependency('libzip')
libjsondep = dependency('json-glib-1.0')
subdir('res')
subdir('reader')
executable('jwlibrary', 'main.c', 'navigation.c', 'drawer.c', 'stack.c', reader_sources, resources,
    dependencies : [webkitdep, gstreamerdep, gstvideodep, sqlitedep, libzipdep, libjsondep],
    install: true,
)