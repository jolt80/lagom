/*
 * Copyright (c) 2018 Tomas Szabo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TOKENDEFINITION_H_
#define TOKENDEFINITION_H_

#include <sstream>
#include <string>

namespace lagom {

enum class Alignment { left, right };

class TokenDefinition {
 public:
  TokenDefinition()
      : name{}
      , width{}
      , alignment{Alignment::left}
      , crop{Alignment::left}
      , initialVisibility{false} {}
  TokenDefinition(std::string _name, int _width, Alignment _alignment, bool _initialVisibility)
      : name{_name}
      , width{_width}
      , alignment{_alignment}
      , crop{Alignment::left}
      , initialVisibility{_initialVisibility} {}
  TokenDefinition(std::string _name, int _width, Alignment _alignment, Alignment _crop, bool _initialVisibility)
      : name{_name}
      , width{_width}
      , alignment{_alignment}
      , crop{_crop}
      , initialVisibility{_initialVisibility} {}
  virtual ~TokenDefinition(){};

  bool operator!=(const TokenDefinition& other) const { return !(*this == other); }

  bool operator==(const TokenDefinition& other) const {
    return name == other.name && width == other.width && alignment == other.alignment && crop == other.crop &&
           initialVisibility == other.initialVisibility;
  }

  inline const std::string& getName() const { return name; }

  inline int getWidth() const { return width; }

  inline Alignment getAlignment() const { return alignment; }

  inline Alignment getCrop() const { return crop; }

  bool getInitialVisibilty() const { return initialVisibility; }

  static std::string alignmentToString(Alignment a) {
    if (a == Alignment::left)
      return "left";
    else
      return "right";
  }

  std::string toString() const;

 private:
  std::string name;
  int width;
  Alignment alignment;
  Alignment crop;
  bool initialVisibility;
};

std::ostream& operator<<(std::ostream& stream, const TokenDefinition& tokenDef);

}  // namespace lagom

#endif /* TOKENDEFINITION_H_ */
