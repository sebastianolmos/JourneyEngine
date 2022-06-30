# Do note that OpenAL32.lib is an import library, you you need to
# place OpenAL32.dll next to the .exe file to find the definitions
# during runtime.
function(Add_Game TARGETNAME FILENAME)
	add_executable(${TARGETNAME} ${FILENAME})
	set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 20)
	set_property(TARGET ${TARGETNAME} PROPERTY FOLDER Games)
	target_link_libraries(${TARGETNAME} PRIVATE 
				JourneyEngine 
				${THIRD_PARTY_LIBRARIES})
	target_include_directories(${TARGETNAME} PRIVATE ${JOURNEY_INCLUDE_DIRECTORY} ${THIRD_PARTY_INCLUDE_DIRECTORIES}) 
endfunction(Add_Game)

Add_Game(LittleDemo_v0 littleDemo.cpp)
Add_Game(CatchGame catchGame.cpp)
Add_Game(TestDemo testDemo.cpp)