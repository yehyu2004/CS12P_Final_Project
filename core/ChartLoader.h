#ifndef CHARTLOADER_H_INCLUDED
#define CHARTLOADER_H_INCLUDED

#include <fstream>
#include "../library/nlohmann/json.hpp"
#include "../objects/Note.h"
#include <queue>

using json = nlohmann::json;

class ChartLoader {
public:
    static ChartLoader *get_instance() {
        static ChartLoader instance;
        return &instance;
    }
    std::queue<Note> load_chart(const std::string chart_name, std::string game_type);
};

#endif
