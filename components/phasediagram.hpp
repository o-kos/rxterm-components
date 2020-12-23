#ifndef PHASE_DIAGRAM_HPP
#define PHASE_DIAGRAM_HPP

#include <rxterm/image.hpp>
#include <rxterm/utils.hpp>
#include <rxterm/style.hpp>
#include <rxterm/reflow.hpp>
#include <algorithm>
#include <numeric>

/*
 01234567890123456789
0⠑⢄⠀⠀⠀⠀⠀⠀|⠀⠀⠀⠀⠀⠀⡠⠊
1⠀⠀⠑⢄⠀⠀⠀⠀|⠀⠀⠀⠀⡠⠊⠀⠀
2⠀⠀⠀⠀⠑⢄⠀⠀|⠀⠀⡠⠊⠀⠀⠀⠀
3 ⠀⠀⠀⠀⠀⠑⢄|⡠⠊⠀⠀⠀⠀⠀⠀
4---------|---------
5⠀⠀⠀⠀⠀⠀⡠⠊|⠑⢄⠀⠀⠀⠀⠀⠀
6⠀⠀⠀⠀⡠⠊⠀⠀|⠀⠀⠑⢄⠀⠀⠀⠀
7⠀⠀⡠⠊⠀⠀⠀⠀|⠀⠀⠀⠀⠑⢄⠀⠀
8⡠⠊⠀⠀⠀⠀⠀⠀|⠀⠀⠀⠀⠀⠀⠑⢄
*/

namespace rxterm {

struct PhaseDiagram {
    Style const style;
    std::string const content;

    template<class T>
    PhaseDiagram(Style const& style, T const& content)
        : style{style}
        , content{toString(content)} {
    }

    template<class T>
    explicit PhaseDiagram(T const& content)
        : style{Style{}}
        , content{toString(content)} {
    }

    [[nodiscard]] Image render(unsigned const /*maxWidth*/) const {
        auto const height = 9;
        auto const width = 19;

        auto image = Image::create(width, height, Pixel{'\0', style});
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image(x, y).c = '+';
            }
        }
        return image;
    }
};

}
#endif
