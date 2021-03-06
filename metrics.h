//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_METRICS_H_
#define UPDATE_ENGINE_METRICS_H_

#include <base/time/time.h>

#include "update_engine/common/certificate_checker.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/error_code.h"

namespace chromeos_update_engine {

class SystemState;

namespace metrics {

// UpdateEngine.Daily.* metrics.
extern const char kMetricDailyOSAgeDays[];

// UpdateEngine.Check.* metrics.
extern const char kMetricCheckDownloadErrorCode[];
extern const char kMetricCheckReaction[];
extern const char kMetricCheckResult[];
extern const char kMetricCheckTimeSinceLastCheckMinutes[];
extern const char kMetricCheckTimeSinceLastCheckUptimeMinutes[];

// UpdateEngine.Attempt.* metrics.
extern const char kMetricAttemptNumber[];
extern const char kMetricAttemptPayloadType[];
extern const char kMetricAttemptPayloadSizeMiB[];
extern const char kMetricAttemptConnectionType[];
extern const char kMetricAttemptDurationMinutes[];
extern const char kMetricAttemptDurationUptimeMinutes[];
extern const char kMetricAttemptTimeSinceLastAttemptSeconds[];
extern const char kMetricAttemptTimeSinceLastAttemptUptimeSeconds[];
extern const char kMetricAttemptPayloadBytesDownloaded[];
extern const char kMetricAttemptPayloadDownloadSpeedKBps[];
extern const char kMetricAttemptDownloadSource[];
extern const char kMetricAttemptResult[];
extern const char kMetricAttemptInternalErrorCode[];
extern const char kMetricAttemptDownloadErrorCode[];

// UpdateEngine.SuccessfulUpdate.* metrics.
extern const char kMetricSuccessfulUpdateAttemptCount[];
extern const char kMetricSuccessfulUpdateBytesDownloadedMiB[];
extern const char kMetricSuccessfulUpdateDownloadOverheadPercentage[];
extern const char kMetricSuccessfulUpdateDownloadSourcesUsed[];
extern const char kMetricSuccessfulUpdatePayloadType[];
extern const char kMetricSuccessfulUpdatePayloadSizeMiB[];
extern const char kMetricSuccessfulUpdateTotalDurationMinutes[];
extern const char kMetricSuccessfulUpdateRebootCount[];
extern const char kMetricSuccessfulUpdateUpdatesAbandonedCount[];
extern const char kMetricSuccessfulUpdateUrlSwitchCount[];

// UpdateEngine.Rollback.* metric.
extern const char kMetricRollbackResult[];

// UpdateEngine.* metrics.
extern const char kMetricFailedUpdateCount[];
extern const char kMetricInstallDateProvisioningSource[];
extern const char kMetricTimeToRebootMinutes[];

// The possible outcomes when checking for updates.
//
// This is used in the UpdateEngine.Check.Result histogram.
enum class CheckResult {
  kUpdateAvailable,    // Response indicates an update is available.
  kNoUpdateAvailable,  // Response indicates no updates are available.
  kDownloadError,      // Error downloading response from Omaha.
  kParsingError,       // Error parsing response.
  kRebootPending,      // No update check was performed a reboot is pending.

  kNumConstants,
  kUnset = -1
};

// Possible ways a device can react to a new update being available.
//
// This is used in the UpdateEngine.Check.Reaction histogram.
enum class CheckReaction {
  kUpdating,    // Device proceeds to download and apply update.
  kIgnored  ,   // Device-policy dictates ignoring the update.
  kDeferring,   // Device-policy dictates waiting.
  kBackingOff,  // Previous errors dictates waiting.

  kNumConstants,
  kUnset = -1
};

// The possible ways that downloading from a HTTP or HTTPS server can fail.
//
// This is used in the UpdateEngine.Check.DownloadErrorCode and
// UpdateEngine.Attempt.DownloadErrorCode histograms.
enum class DownloadErrorCode {
  // Errors that can happen in the field. See http://crbug.com/355745
  // for how we plan to add more detail in the future.
  kDownloadError = 0,  // Error downloading data from server.

  // IMPORTANT: When adding a new error code, add at the bottom of the
  // above block and before the kInputMalformed field. This
  // is to ensure that error codes are not reordered.

  // This error code is used to convey that malformed input was given
  // to the utils::GetDownloadErrorCode() function. This should never
  // happen but if it does it's because of an internal update_engine
  // error and we're interested in knowing this.
  kInputMalformed = 100,

  // Bucket for capturing HTTP status codes not in the 200-599
  // range. This should never happen in practice but if it does we
  // want to know.
  kHttpStatusOther = 101,

  // Above 200 and below 600, the value is the HTTP status code.
  kHttpStatus200 = 200,

  kNumConstants = 600,

  kUnset = -1
};

// Possible ways an update attempt can end.
//
// This is used in the UpdateEngine.Attempt.Result histogram.
enum class AttemptResult {
  kUpdateSucceeded,             // The update succeeded.
  kInternalError,               // An internal error occurred.
  kPayloadDownloadError,        // Failure while downloading payload.
  kMetadataMalformed,           // Metadata was malformed.
  kOperationMalformed,          // An operation was malformed.
  kOperationExecutionError,     // An operation failed to execute.
  kMetadataVerificationFailed,  // Metadata verification failed.
  kPayloadVerificationFailed,   // Payload verification failed.
  kVerificationFailed,          // Root or Kernel partition verification failed.
  kPostInstallFailed,           // The postinstall step failed.
  kAbnormalTermination,         // The attempt ended abnormally.
  kUpdateCanceled,              // Update canceled by the user.

  kNumConstants,

  kUnset = -1
};

// Possible ways the device is connected to the Internet.
//
// This is used in the UpdateEngine.Attempt.ConnectionType histogram.
enum class ConnectionType {
  kUnknown,           // Unknown.
  kEthernet,          // Ethernet.
  kWifi,              // Wireless.
  kWimax,             // WiMax.
  kBluetooth,         // Bluetooth.
  kCellular,          // Cellular.
  kTetheredEthernet,  // Tethered (Ethernet).
  kTetheredWifi,      // Tethered (Wifi).

  kNumConstants,
  kUnset = -1
};

// Possible ways a rollback can end.
//
// This is used in the UpdateEngine.Rollback histogram.
enum class RollbackResult {
  kFailed,
  kSuccess,

  kNumConstants
};

// Helper function to report metrics related to rollback. The
// following metrics are reported:
//
//  |kMetricRollbackResult|
void ReportRollbackMetrics(SystemState *system_state,
                           RollbackResult result);

// Helper function to report metrics reported once a day. The
// following metrics are reported:
//
//  |kMetricDailyOSAgeDays|
void ReportDailyMetrics(SystemState *system_state,
                        base::TimeDelta os_age);

// Helper function to report metrics after completing an update check
// with the ChromeOS update server ("Omaha"). The following metrics
// are reported:
//
//  |kMetricCheckResult|
//  |kMetricCheckReaction|
//  |kMetricCheckDownloadErrorCode|
//  |kMetricCheckTimeSinceLastCheckMinutes|
//  |kMetricCheckTimeSinceLastCheckUptimeMinutes|
//
// The |kMetricCheckResult| metric will only be reported if |result|
// is not |kUnset|.
//
// The |kMetricCheckReaction| metric will only be reported if
// |reaction| is not |kUnset|.
//
// The |kMetricCheckDownloadErrorCode| will only be reported if
// |download_error_code| is not |kUnset|.
//
// The values for the |kMetricCheckTimeSinceLastCheckMinutes| and
// |kMetricCheckTimeSinceLastCheckUptimeMinutes| metrics are
// automatically reported and calculated by maintaining persistent
// and process-local state variables.
void ReportUpdateCheckMetrics(SystemState *system_state,
                              CheckResult result,
                              CheckReaction reaction,
                              DownloadErrorCode download_error_code);


// Helper function to report metrics after the completion of each
// update attempt. The following metrics are reported:
//
//  |kMetricAttemptNumber|
//  |kMetricAttemptPayloadType|
//  |kMetricAttemptPayloadSizeMiB|
//  |kMetricAttemptDurationSeconds|
//  |kMetricAttemptDurationUptimeSeconds|
//  |kMetricAttemptTimeSinceLastAttemptMinutes|
//  |kMetricAttemptTimeSinceLastAttemptUptimeMinutes|
//  |kMetricAttemptPayloadBytesDownloadedMiB|
//  |kMetricAttemptPayloadDownloadSpeedKBps|
//  |kMetricAttemptDownloadSource|
//  |kMetricAttemptResult|
//  |kMetricAttemptInternalErrorCode|
//  |kMetricAttemptDownloadErrorCode|
//
// The |kMetricAttemptInternalErrorCode| metric will only be reported
// if |internal_error_code| is not |kErrorSuccess|.
//
// The |kMetricAttemptDownloadErrorCode| metric will only be
// reported if |payload_download_error_code| is not |kUnset|.
//
// The values for the |kMetricAttemptTimeSinceLastAttemptMinutes| and
// |kMetricAttemptTimeSinceLastAttemptUptimeMinutes| metrics are
// automatically calculated and reported by maintaining persistent and
// process-local state variables.
void ReportUpdateAttemptMetrics(
    SystemState *system_state,
    int attempt_number,
    PayloadType payload_type,
    base::TimeDelta duration,
    base::TimeDelta duration_uptime,
    int64_t payload_size,
    int64_t payload_bytes_downloaded,
    int64_t payload_download_speed_bps,
    DownloadSource download_source,
    AttemptResult attempt_result,
    ErrorCode internal_error_code,
    DownloadErrorCode payload_download_error_code,
    ConnectionType connection_type);

// Reports the |kAbnormalTermination| for the |kMetricAttemptResult|
// metric. No other metrics in the UpdateEngine.Attempt.* namespace
// will be reported.
void ReportAbnormallyTerminatedUpdateAttemptMetrics(SystemState *system_state);

// Helper function to report the after the completion of a successful
// update attempt. The following metrics are reported:
//
//  |kMetricSuccessfulUpdateAttemptCount|
//  |kMetricSuccessfulUpdateUpdatesAbandonedCount|
//  |kMetricSuccessfulUpdatePayloadType|
//  |kMetricSuccessfulUpdatePayloadSizeMiB|
//  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpsServer|
//  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpServer|
//  |kMetricSuccessfulUpdateBytesDownloadedMiBHttpPeer|
//  |kMetricSuccessfulUpdateBytesDownloadedMiB|
//  |kMetricSuccessfulUpdateDownloadSourcesUsed|
//  |kMetricSuccessfulUpdateDownloadOverheadPercentage|
//  |kMetricSuccessfulUpdateTotalDurationMinutes|
//  |kMetricSuccessfulUpdateRebootCount|
//  |kMetricSuccessfulUpdateUrlSwitchCount|
//
// The values for the |kMetricSuccessfulUpdateDownloadSourcesUsed| are
// |kMetricSuccessfulUpdateBytesDownloadedMiB| metrics automatically
// calculated from examining the |num_bytes_downloaded| array.
void ReportSuccessfulUpdateMetrics(
    SystemState *system_state,
    int attempt_count,
    int updates_abandoned_count,
    PayloadType payload_type,
    int64_t payload_size,
    int64_t num_bytes_downloaded[kNumDownloadSources],
    int download_overhead_percentage,
    base::TimeDelta total_duration,
    int reboot_count,
    int url_switch_count);

// Helper function to report the after the completion of a SSL certificate
// check. One of the following metrics is reported:
//
//  |kMetricCertificateCheckUpdateCheck|
//  |kMetricCertificateCheckDownload|
void ReportCertificateCheckMetrics(SystemState* system_state,
                                   ServerToCheck server_to_check,
                                   CertificateCheckResult result);

}  // namespace metrics

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_METRICS_H_
