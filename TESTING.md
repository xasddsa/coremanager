# Manual Test Cases for CPUSpeedManager

## 1. UI and Information Display Verification

**Test Case ID:** TC_UI_001
**Category:** UI and Information Display
**Description:** Verify correct CPU core count is displayed upon application startup.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
    3. Observe the label indicating total CPU cores (e.g., "CPU Cores: X").
**Expected Result:** The displayed core count matches the system's actual core count (e.g., as reported by `nproc` or `lscpu`).

**Test Case ID:** TC_UI_002
**Category:** UI and Information Display
**Description:** Verify overall CPU frequency for the primary core is displayed.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
    3. Observe the label "Overall CPU Frequency (Core X): YYYY MHz".
**Expected Result:** The frequency displayed should be a plausible value for the primary CPU core. (Verification: `cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq` divided by 1000).

**Test Case ID:** TC_UI_003
**Category:** UI and Information Display
**Description:** Verify CPU governor/frequency dropdown is populated for the primary core.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
    3. Click on the "CPU Governor/Frequency:" dropdown.
**Expected Result:** The dropdown lists available frequencies (e.g., "2200 MHz") and governors (e.g., "Governor: performance"). The current setting for the primary core should be pre-selected. (Verification: check contents of `/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies` and `/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors`).

**Test Case ID:** TC_UI_004
**Category:** UI and Information Display
**Description:** Verify CPU core list displays status for all cores.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
    3. Observe the list under "CPU Cores:".
**Expected Result:** Each CPU core is listed with its status (e.g., "Core 0: Online - YYYY MHz", "Core 1: Offline"). Checkboxes are present for cores > 0. Core 0 checkbox is disabled. (Verification: check `/sys/devices/system/cpu/cpu*/online` and `/sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq`).

**Test Case ID:** TC_UI_005
**Category:** UI and Information Display
**Description:** Verify UI elements are disabled if initial CPU core count detection fails.
**Steps:**
    1. (Simulate failure if possible, e.g., by temporarily restricting access to /sys/devices/system/cpu if testing environment allows).
    2. Build the application if you haven't already (e.g., run `./build.sh`).
    3. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
**Expected Result:** A critical error message is shown. "Overall CPU Frequency" and "CPU Cores" labels show "Error". Dropdown, core list, and "Apply Changes" button are disabled.

## 2. CPU Governor Change Functionality

**Test Case ID:** TC_FUNC_GOV_001
**Category:** CPU Governor Change
**Description:** Change CPU governor to 'performance' when running as non-root.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Select "Governor: performance" from the dropdown.
    3. Click "Apply Changes".
    4. Authenticate with `pkexec` when prompted.
    5. Observe the current governor/frequency display in the UI after refresh.
**Expected Result:**
    1. `pkexec` prompts for password.
    2. UI updates to show 'performance' as the current governor for the primary core in the dropdown.
    3. All online cores' list entries reflect the new governor (if applicable to the display format) or frequency associated with that governor.
    4. (Verification) `/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor` for all online cores shows 'performance'.

**Test Case ID:** TC_FUNC_GOV_002
**Category:** CPU Governor Change
**Description:** Change CPU governor to 'powersave' when running as root.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager with root privileges (e.g., `sudo ./build/CPUSpeedManager`).
    3. Select "Governor: powersave" from the dropdown.
    4. Click "Apply Changes".
**Expected Result:**
    1. No `pkexec` prompt appears.
    2. UI updates to show 'powersave' as the current governor for the primary core.
    3. All online cores' list entries reflect the new governor/frequency.
    4. (Verification) `/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor` for all online cores shows 'powersave'.

**Test Case ID:** TC_FUNC_GOV_003
**Category:** CPU Governor Change
**Description:** Attempt to change CPU governor with incorrect/cancelled `pkexec` authentication.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Select a different governor from the dropdown.
    4. Click "Apply Changes".
    4. When `pkexec` prompts, enter an incorrect password or cancel the dialog.
**Expected Result:**
    1. An error message box is displayed indicating the failure to set the governor.
    2. The UI does not change the governor selection in the dropdown (or reverts to the actual current state after refresh).
    3. (Verification) `/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor` remain unchanged.

## 3. CPU Frequency Change Functionality

**Test Case ID:** TC_FUNC_FREQ_001
**Category:** CPU Frequency Change
**Description:** Change CPU frequency when running as non-root (implies setting 'userspace' governor).
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Ensure 'userspace' governor is available. If not, this test may need adjustment or be N/A.
    4. Select a specific frequency (e.g., "1800 MHz") from the dropdown.
    4. Click "Apply Changes".
    5. Authenticate with `pkexec` when prompted (potentially twice: once for 'userspace', once for frequency).
    6. Observe the current governor/frequency display in the UI.
**Expected Result:**
    1. `pkexec` prompts for password.
    2. UI updates to show the selected frequency as current (and 'userspace' or the selected frequency in governor part of dropdown if it changes).
    3. All online cores' list entries reflect the new frequency.
    4. (Verification) `/sys/devices/system/cpu/cpu*/cpufreq/scaling_governor` for all online cores shows 'userspace'.
    5. (Verification) `/sys/devices/system/cpu/cpu*/cpufreq/scaling_setspeed` or `/sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq` reflects the chosen frequency for all online cores.

**Test Case ID:** TC_FUNC_FREQ_002
**Category:** CPU Frequency Change
**Description:** Change CPU frequency when running as root.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager with root privileges (e.g., `sudo ./build/CPUSpeedManager`).
    3. Select a specific frequency from the dropdown.
    4. Click "Apply Changes".
**Expected Result:**
    1. No `pkexec` prompt.
    2. UI updates to show the selected frequency.
    3. All online cores reflect the new frequency.
    4. (Verification) Governor is 'userspace', and frequency matches selection for all online cores.

**Test Case ID:** TC_FUNC_FREQ_003
**Category:** CPU Frequency Change
**Description:** Attempt to set frequency not supported by current governor (other than 'userspace').
**Steps:**
    1. Ensure current governor is *not* 'userspace' (e.g., 'performance').
    2. Build the application if you haven't already (e.g., run `./build.sh`).
    3. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager` or `sudo ./build/CPUSpeedManager` if root privileges are required by the test).
    4. Select a frequency from the dropdown (which should ideally only be enabled if 'userspace' is set, but test the attempt).
    5. Click "Apply Changes".
**Expected Result:**
    1. An error message box should be displayed if the operation fails (as setting frequency directly usually requires 'userspace' governor).
    2. The frequency should not change. The application attempts to set 'userspace' first, this part should be verified. If that succeeds, then frequency setting is attempted.
    3. (Verification) Governor and frequency remain unchanged if the 'userspace' switch fails or is not allowed by the current governor. Or, governor changes to 'userspace' and frequency changes.

## 4. CPU Core Online/Offline Functionality

**Test Case ID:** TC_FUNC_CORE_001
**Category:** CPU Core Online/Offline
**Description:** Take a CPU core (e.g., Core 1) offline when running as non-root.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Ensure Core 1 (or any core > 0) is online.
    4. Uncheck the checkbox next to "Core 1".
    4. Click "Apply Changes".
    5. Authenticate with `pkexec` when prompted.
    6. Observe the status of Core 1 in the list.
**Expected Result:**
    1. `pkexec` prompts for password.
    2. UI updates to show "Core 1: Offline".
    3. (Verification) `/sys/devices/system/cpu/cpu1/online` shows '0'.

**Test Case ID:** TC_FUNC_CORE_002
**Category:** CPU Core Online/Offline
**Description:** Bring a CPU core (e.g., Core 1) online when running as root.
**Steps:**
    1. Manually take Core 1 offline (e.g., `echo 0 | sudo tee /sys/devices/system/cpu/cpu1/online`).
    2. Build the application if you haven't already (e.g., run `./build.sh`).
    3. Launch CPUSpeedManager with root privileges (e.g., `sudo ./build/CPUSpeedManager`).
    4. Core 1 should be shown as offline and unchecked.
    5. Check the checkbox next to "Core 1".
    5. Click "Apply Changes".
**Expected Result:**
    1. No `pkexec` prompt.
    2. UI updates to show "Core 1: Online - YYYY MHz".
    3. (Verification) `/sys/devices/system/cpu/cpu1/online` shows '1'.

**Test Case ID:** TC_FUNC_CORE_003
**Category:** CPU Core Online/Offline
**Description:** Verify Core 0 cannot be taken offline.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager` or `sudo ./build/CPUSpeedManager` if root privileges are required by the test).
    3. Observe the checkbox for "Core 0".
**Expected Result:** The checkbox for Core 0 is disabled and checked (assuming Core 0 is online). It cannot be unchecked. Clicking "Apply Changes" with any other settings should not affect Core 0's online status.

**Test Case ID:** TC_FUNC_CORE_004
**Category:** CPU Core Online/Offline
**Description:** Attempt to change core online state with incorrect/cancelled `pkexec` authentication.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Uncheck a core (e.g., Core 1).
    4. Click "Apply Changes".
    4. When `pkexec` prompts, enter an incorrect password or cancel the dialog.
**Expected Result:**
    1. An error message box is displayed indicating the failure.
    2. The core's online status in the UI remains unchanged (or reverts after refresh).
    3. (Verification) The core's `/sys/devices/system/cpu/cpuX/online` status remains unchanged.

## 5. Privilege Handling

**Test Case ID:** TC_PRIV_001
**Category:** Privilege Handling
**Description:** Application functions correctly when launched with root privileges.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager using `sudo ./build/CPUSpeedManager`.
    3. Perform various operations: change governor, change frequency (if 'userspace' selected), take core offline/online (not Core 0).
    4. Click "Apply Changes".
**Expected Result:** All operations succeed without `pkexec` prompts. UI updates correctly. System state changes as expected.

**Test Case ID:** TC_PRIV_002
**Category:** Privilege Handling
**Description:** Application uses `pkexec` when launched as non-root.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Perform various operations that require privilege: change governor, change frequency, take core offline/online.
    4. Click "Apply Changes".
**Expected Result:** `pkexec` password prompt appears for each category of change that needs elevation. Operations succeed after correct authentication.

**Test Case ID:** TC_PRIV_003
**Category:** Privilege Handling
**Description:** Verify `pkexec` is not called if no settings requiring privilege are changed.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager without root privileges (e.g., `./build/CPUSpeedManager`).
    3. Do not change any settings, or only change settings that do not require privileges (if any such settings existed).
    4. Click "Apply Changes" (button might be inactive if no changes are made, this depends on implementation detail not specified).
    4. Alternatively, change a setting then change it back to its original value. Click "Apply Changes".
**Expected Result:** `pkexec` prompt does not appear if the net changes do not require privilege elevation. (Note: current implementation applies all settings, so this might always trigger pkexec if any setting is different from current state). This test case might highlight that the app always tries to write all settings.

## 6. Error Message Display

**Test Case ID:** TC_ERR_001
**Category:** Error Message Display
**Description:** Verify error message for failure to set governor.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager as non-root (e.g., `./build/CPUSpeedManager`).
    3. Select a governor.
    4. Click "Apply Changes".
    4. Cancel `pkexec` dialog.
**Expected Result:** A `QMessageBox` (warning) appears, detailing that setting the governor failed, and includes error/output from the `pkexec` attempt if available.

**Test Case ID:** TC_ERR_002
**Category:** Error Message Display
**Description:** Verify error message for failure to set frequency.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager as non-root (e.g., `./build/CPUSpeedManager`).
    3. Select a frequency.
    4. Click "Apply Changes".
    4. Cancel `pkexec` dialog (for setting 'userspace' or the frequency itself).
**Expected Result:** A `QMessageBox` (warning) appears, detailing that setting the frequency (or prerequisite 'userspace' governor) failed.

**Test Case ID:** TC_ERR_003
**Category:** Error Message Display
**Description:** Verify error message for failure to change core online status.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager as non-root (e.g., `./build/CPUSpeedManager`).
    3. Uncheck Core 1.
    4. Click "Apply Changes".
    4. Cancel `pkexec` dialog.
**Expected Result:** A `QMessageBox` (warning) appears, detailing that changing the core's online status failed.

**Test Case ID:** TC_ERR_004
**Category:** Error Message Display
**Description:** Verify critical error message if CPU core count cannot be determined at startup.
**Steps:**
    1. (Requires simulation of unreadable `/sys/devices/system/cpu/`)
    2. Build the application if you haven't already (e.g., run `./build.sh`).
    3. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
**Expected Result:** `QMessageBox::critical` is shown: "Fatal Error", "Could not determine CPU core count...". UI controls are disabled.

## 7. Basic Usability

**Test Case ID:** TC_USAB_001
**Category:** Basic Usability
**Description:** Application launches successfully.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Execute the CPUSpeedManager application (e.g., `./build/CPUSpeedManager`).
**Expected Result:** The main window appears without crashing.

**Test Case ID:** TC_USAB_002
**Category:** Basic Usability
**Description:** UI elements are responsive.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager`).
    3. Click on dropdowns, list items (if interactive beyond checkbox), buttons.
**Expected Result:** UI elements respond to clicks (e.g., dropdown opens, button clicks).

**Test Case ID:** TC_USAB_003
**Category:** Basic Usability
**Description:** "Apply Changes" button triggers actions.
**Steps:**
    1. Build the application if you haven't already (e.g., run `./build.sh`).
    2. Launch CPUSpeedManager (e.g., `./build/CPUSpeedManager` or `sudo ./build/CPUSpeedManager` if root privileges are required by the test).
    3. Make a change (e.g., select a different governor).
    4. Click "Apply Changes".
**Expected Result:** The application attempts to apply the settings (e.g., prompts for `pkexec` if non-root). Information in UI refreshes.
```
