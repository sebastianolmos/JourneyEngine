set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
define_property(
    TARGET
    PROPERTY FOLDER
    INHERITED
    BRIEF_DOCS "Set the folder name."
    FULL_DOCS  "Use to organize targets in an IDE."
)

function(add_subdirectory_with_folder _folder_name _folder)
    add_subdirectory(${_folder} ${ARGN})
    set_property(DIRECTORY "${_folder}" PROPERTY FOLDER "${_folder_name}")
endfunction()

add_subdirectory_with_folder("libs" glad/opengl3.3/source/glad)
add_subdirectory_with_folder("libs" glfw/source/glfw)
add_subdirectory_with_folder("libs/OpenAl" openal/source/openal-soft)
add_subdirectory_with_folder("libs" dr_wav)
add_subdirectory_with_folder("libs/Assimp" assimp/source/assimp)
add_subdirectory_with_folder("libs" stb)