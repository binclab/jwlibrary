project('jwlibrary', 'c')
webkitdep = dependency('webkitgtk-6.0')
gstreamerdep = dependency('gstreamer-1.0')
gstvideodep = dependency('gstreamer-video-1.0')
sqlitedep = dependency('sqlite3')
openssldep = dependency('openssl')
libzipdep = dependency('libzip')
libjsondep = dependency('json-glib-1.0')
libxmldep = dependency('libxml-2.0')
zlibdep = dependency('zlib')
subdir('res')
subdir('reader')
executable('jwlibrary', 'main.c', 'navigation.c', 'drawer.c', 'stack.c', 'home.c', reader_sources, resources,
    dependencies : [webkitdep, gstreamerdep, gstvideodep, sqlitedep, openssldep, libzipdep, libjsondep, zlibdep, libxmldep],
    install: true,
)