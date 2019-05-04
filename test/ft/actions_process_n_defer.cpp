#include <boost/sml.hpp>
#include <deque>
#include <queue>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;
const auto s4 = sml::state<class s4>;
const auto s5 = sml::state<class s5>;
const auto s6 = sml::state<class s6>;
const auto s7 = sml::state<class s7>;

test mix_process_n_defer_at_init = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + on_entry<sml::initial> / process(e1{})
        , s1 + event<e1> / defer = s2
        , s2 / defer = s3
        , s3 + event<e1> / process(e2{})
        , s3 + event<e2> / defer = s4
        , s4 + event<e2> = s5
        , s5 = s6
        , s6 + on_entry<_> / process(e3{})
        , s6 + event<e3> = s7
        , s7 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>, sml::defer_queue<std::deque>> sm{};
  expect(sm.is(sml::X));
};

test mix_process_n_defer = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + event<e1> / defer = s2
        , s2 / defer = s3
        , s3 + event<e1> / process(e2{})
        , s3 + event<e2> / defer = s4
        , s4 + event<e2> = s5
        , s5 = s6
        , s6 + on_entry<_> / process(e3{})
        , s6 + event<e3> = s7
        , s7 = X
      );
      // clang-format on
    }
  };  // internal, defer, process, defer, internal, process, internal

  sml::sm<c, sml::process_queue<std::queue>, sml::defer_queue<std::deque>> sm{};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};