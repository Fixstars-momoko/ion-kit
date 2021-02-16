#ifndef ION_PARAM_H
#define ION_PARAM_H

#include <string>

#include <nlohmann/json.hpp>

namespace ion {

using json = nlohmann::json;

/**
 * Param class is used to create static parameter for each node.
 */
class Param {
public:
    friend void to_json(json &, const Param &);
    friend void from_json(const json &, Param &);

    Param() {
    }

    /**
      * Create static parameter which is passed as GeneratorParam declared in user-defined class deriving BuildingBlock.
      * @arg key: Key of the parameter.
      * It should be matched with first argument of GeneratorParam declared in user-defined class deriving BuildingBlock.
      * @arg val: Value in string.
      * It can be string representation which is able to convert through std::istringstream.
      */
    Param(const std::string &key, const std::string &val)
        : key_(key), val_(val) {
    }

    std::string key() const {
        return key_;
    }
    std::string val() const {
        return val_;
    }

private:
    std::string key_;
    std::string val_;
};

}  // namespace ion

#endif  // ION_PARAM_H
