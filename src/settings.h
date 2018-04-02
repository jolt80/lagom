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

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <re2/re2.h>
#include <vector>
#include "log_line_tokenizer.h"
#include "string_literal.h"
#include "token_definition.h"

namespace lagom {

/**
 * Represents settings for the logline tokenizer.
 */
class Settings {
 public:
  Settings();
  Settings(std::string filePath);
  virtual ~Settings();

  bool operator==(const Settings& other) const;

  const std::vector<LogLineTokenizer*> getTokenizers() const { return tokenizers; }

  const TokenDefinition& getTokenDefinition(int tokenIndex) const;

  std::string toString() const;

  static void writeDefaultSettingsFile(std::string filePath);

 private:
  bool buildTokenDefinition(int index, StringLiteral line);
  bool buildTokenMatcherSettings(StringLiteral line, TokenMatcherSettings& matcherSettings);

  std::vector<LogLineTokenizer*> tokenizers;
  std::array<TokenDefinition, 10> tokens;
};

std::ostream& operator<<(std::ostream& stream, const Settings& settings);

} // namespace lagom

#endif /* SETTINGS_H_ */
