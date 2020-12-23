#ifndef LOG_VIEW_HPP
#define LOG_VIEW_HPP

#include <rxterm/image.hpp>

#include <list>
#include <string>
#include <utility>

namespace rxterm {

struct LogView {
    Style const style;
    std::vector<std::string> content;
    unsigned lines;

    LogView(Style const &style, std::vector<std::string> content, unsigned lines)
        : style{style}
        , content{std::move(content)}
        , lines{lines} {
    }

    LogView(std::vector<std::string> content, unsigned lines)
        : style{Style{}}
        , content{std::move(content)}
        , lines{lines} {
    }

    [[nodiscard]] Image render(unsigned const maxWidth) const {
        auto const height = lines;
        auto const width = maxWidth;

        auto image = Image::create(width, height, Pixel{' ', style});
        int y = 0;
        for (auto it = content.cend() - std::min<unsigned>(content.size(), lines); it < content.end(); ++it) {
            auto line = *it;
            for (int x = 0; x < line.size(); ++x) {
                image(x, y).c = line[x];
            }
            ++y;
        }

        return image;
    }

};

}
#endif
