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

#ifndef LAGOM_LOGVIEWREPOSITORY_H_
#define LAGOM_LOGVIEWREPOSITORY_H_

#include <list>
#include <memory>
#include "filtered_log_view.h"

namespace lagom {

/**
 * Use to create FilteredLogView objects.
 *
 * Caches already created filtered log views for reuse.
 */
class FilteredLogViewFactory {
 public:
  FilteredLogViewFactory(Log& _log)
      : log(_log){};
  virtual ~FilteredLogViewFactory() = default;

  std::shared_ptr<LogView> getFilteredLogView(std::string pattern);

  static bool isMultiplePattern(std::string& pattern);

  std::string getLastErrorMessage();

 private:
  std::string errorMessage;
  std::vector<int>* buildVectorOfMatchingLines(std::string pattern);
  std::list<std::string> splitMultiplePattern(std::string& pattern);

  Log& log;
  std::map<std::string, std::shared_ptr<FilteredLogView>> filteredViews;
};

}  // namespace lagom

#endif /* LAGOM_LOGVIEWREPOSITORY_H_ */
