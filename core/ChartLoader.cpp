#include "ChartLoader.h"
#include <iostream>

std::queue<Note> ChartLoader::load_chart(const std::string chart_path, std::string game_type){
    const std::string filename = chart_path;
    std::ifstream file(filename);

    std::cout << filename << "\n";
    // GAME_ASSERT(file.is_open(), "Failed to open chart: %s", filename.c_str());

    json chart_json;
    file >> chart_json;

    std::queue<Note> notes;

    // Accessing 'hit_objects' array
	json note_objects = chart_json["note_objects"];

	for (const auto& obj : note_objects) {
	    double time = obj["time"]; // assume it's sorted
	    int column = obj["column"];
	    std::string type = obj["type"];
	    double duration = 0;

	    // std::cout << "NOTE " << time << " " << column << " " << type << "\n";
	    
	    // If the type is 'hold', access 'duration'
	    if (type == "hold") {
	        duration = obj["duration"];
	        // std::cout << "Duration: " << duration << " ms" << std::endl;
	    }

	    notes.push(Note(time, column, type, game_type, duration));
	}
    return notes;
}
