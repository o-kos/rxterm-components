#include "loren.h"

#include <thread>
#include <chrono>

#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>

#include "components/logview.hpp"
#include "components/phasediagram.hpp"

using namespace rxterm;

int main() {
    using namespace std;
    using namespace chrono_literals;
    using namespace string_literals;

    struct State {
        string header;

        struct Log {
            vector<string> buffer;
            void push(const string &line) {
                if (buffer.size() > 100) buffer.erase(buffer.begin());
                buffer.push_back(line);
            }
        } log;

        struct Progress {
            string title;
            unsigned position;
            unsigned limit;
            Progress() : position{0}, limit{0} {}
            [[nodiscard]] auto value() const { return float(position) / float(limit); }
            [[nodiscard]] string percent() const {
                char buf[6];
                snprintf(buf, sizeof(buf), "%3d%% ", lround(value() * 100));
                return buf;
            }
            [[nodiscard]] string suffix() const {
                return " 7568/10000 [00:33<00:10, 229.00it/s] ";
            }
        } progress;

        struct Status {
            std::string title;
            unsigned value;
            Status() : value{0} {}
        } status;

        int startTime;

        State() : startTime{0} {}
    } state;

    auto renderToTerm = [](auto const &vt, const State &state, unsigned width) {

        struct Layout {
            struct {
                unsigned prefix, body, suffix;
            } pb;
            unsigned log;
            unsigned pd;
            Layout(const State &state, unsigned width) : pd{19} {
                pb.prefix = state.progress.title.size() + state.progress.percent().size();
                pb.suffix = state.progress.suffix().size();
                pb.body = width - pd - pb.prefix - pb.suffix;
                log = width - pd - 1;
            }
        } l{state, width};

        FlowLayout<> layout{
            StackLayout<>{
                Text{state.header},
                FlowLayout<>{
                    Text{state.progress.title},
                    Text{state.progress.percent()},
                    MaxWidth{l.pb.body, Progress{state.progress.value()}},
                    Text{{FontColor::Blue}, state.progress.suffix()}
                },
                FlowLayout<>{
                    Text{state.status.title},
                    Text{{FontColor::Magenta}, state.status.value}
                },
                MaxWidth{l.log, LogView{{Color::Yellow, FontColor::Magenta}, state.log.buffer, 6}}
            },
            PhaseDiagram{"hhj"}
        };
        return vt.flip(layout.render(width).toString());
    };


    VirtualTerminal vt;
    auto w = VirtualTerminal::width();
    if (!w) w = 120;

    state.header = "Data processor";
    state.status.title = "Initializing...";
    state.progress.title = "Process ";
    state.progress.limit = 64 * 1024 * 1024;

    vt = renderToTerm(vt, state, w);
    std::this_thread::sleep_for(200ms);

    state.status.title = "Total steps: ";
    for (;;) {
        state.progress.position += 64 * 1024 * 10;
        state.status.value++;
        state.log.push(nextLoren());

        vt = renderToTerm(vt, state, w);
        if (state.progress.position >= state.progress.limit) break;
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
