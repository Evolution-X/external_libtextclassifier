/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// TODO(smillius): Move intent generation code outside of utils.

#ifndef LIBTEXTCLASSIFIER_UTILS_INTENTS_INTENT_GENERATOR_H_
#define LIBTEXTCLASSIFIER_UTILS_INTENTS_INTENT_GENERATOR_H_

#include <jni.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "annotator/types.h"
#include "utils/intents/intent-config_generated.h"
#include "utils/java/jni-cache.h"
#include "utils/java/scoped_local_ref.h"
#include "utils/optional.h"
#include "utils/strings/stringpiece.h"

namespace libtextclassifier3 {

// A template with parameters for an Android remote action.
struct RemoteActionTemplate {
  // Title shown for the action (see: RemoteAction.getTitle).
  Optional<std::string> title;

  // Description shown for the action (see: RemoteAction.getContentDescription).
  Optional<std::string> description;

  // The action to set on the Intent (see: Intent.setAction).
  Optional<std::string> action;

  // The data to set on the Intent (see: Intent.setData).
  Optional<std::string> data;

  // The type to set on the Intent (see: Intent.setType).
  Optional<std::string> type;

  // Flags for launching the Intent (see: Intent.setFlags).
  Optional<int> flags;

  // Categories to set on the Intent (see: Intent.addCategory).
  std::vector<std::string> category;

  // Explicit application package to set on the Intent (see: Intent.setPackage).
  Optional<std::string> package_name;

  // The list of all the extras to add to the Intent.
  std::map<std::string, Variant> extra;

  // Private request code ot use for the Intent.
  Optional<int> request_code;
};

// Helper class to generate Android intents for text classifier results.
class IntentGenerator {
 public:
  explicit IntentGenerator(const IntentFactoryModel* options,
                           const std::shared_ptr<JniCache>& jni_cache,
                           const jobject context);

  // Generate intents for a classification result.
  std::vector<RemoteActionTemplate> GenerateIntents(
      const ClassificationResult& classification, int64 reference_time_ms_utc,
      StringPiece entity_text) const;

 private:
  const IntentFactoryModel* options_;
  std::shared_ptr<JniCache> jni_cache_;
  const jobject context_;
  std::map<std::string, std::string> generators_;
};

}  // namespace libtextclassifier3

#endif  // LIBTEXTCLASSIFIER_UTILS_INTENTS_INTENT_GENERATOR_H_