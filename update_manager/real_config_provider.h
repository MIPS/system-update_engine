// Copyright 2014 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_

#include <memory>
#include <string>

#include "update_engine/hardware_interface.h"
#include "update_engine/update_manager/config_provider.h"
#include "update_engine/update_manager/generic_variables.h"

namespace chromeos_update_manager {

// ConfigProvider concrete implementation.
class RealConfigProvider : public ConfigProvider {
 public:
  explicit RealConfigProvider(
      chromeos_update_engine::HardwareInterface* hardware)
      : hardware_(hardware) {}

  // Initializes the provider and returns whether it succeeded.
  bool Init();

  Variable<bool>* var_is_oobe_enabled() override {
    return var_is_oobe_enabled_.get();
  }

 private:
  friend class UmRealConfigProviderTest;

  // Used for testing. Sets the root prefix, which is by default "". Call this
  // method before calling Init() in order to mock out the place where the files
  // are being read from.
  void SetRootPrefix(const std::string& prefix) {
    root_prefix_ = prefix;
  }

  std::unique_ptr<ConstCopyVariable<bool>> var_is_oobe_enabled_;

  chromeos_update_engine::HardwareInterface* hardware_;

  // Prefix to prepend to the file paths. Useful for testing.
  std::string root_prefix_;

  DISALLOW_COPY_AND_ASSIGN(RealConfigProvider);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_REAL_CONFIG_PROVIDER_H_