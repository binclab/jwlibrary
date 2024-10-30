resources = custom_target(
    'resources.c',
    output : 'resources.c',
    input: ['gresources.xml', 'script.js'],
  command : [
    find_program('glib-compile-resources'), 
    '--target=@OUTPUT@', 
    '--sourcedir=@CURRENT_SOURCE_DIR@' , 
    '--generate-source', '@CURRENT_SOURCE_DIR@/gresources.xml'
  ],
  build_by_default : true,
)