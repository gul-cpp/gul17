# Find all out tools and their sub-dependencies

# This is possibly used by the debian installation stuff
changelogger = find_program('changelogger', required : false)

# This is used by the Doxygen generation stuff
# doxygen is used by doxywrap; if we have both we are ready to rumble
if get_option('docs')
    doxygen = find_program('doxygen', required : false)
    if doxygen.found()
        graphviz = find_program('dot', required : false)
        if graphviz.found()
            doxy_run = find_program('doxywrap')
        else
            message('Hint: dot is usually a part of the graphviz package')
        endif
    endif
endif

# vi:ts=4:sw=4:sts=4:et:syn=conf
