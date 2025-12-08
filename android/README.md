# Send key event to android device
```sh
adb shell input keyevent KEYCODE_HOME
```
# Run cuttlefish
```sh
HOME=$(pwd) ./bin/launch_cvd  -enable-sandbox=false -start_vnc_server=true
```
# Package super.img
```sh
source build/envsetup.sh
lunch <PRODUCT_NAME>-<BUILD_VARIANT>
python vendor/qcom/opensource/core-utils/build/build_image_standalone.py --image super --qssi_build_path ../android --target_build_path . --merged_build_path . --target_lunch connect_w --no_tmp --output_ota
```