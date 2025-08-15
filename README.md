# Bluefox2

| Build status | [![Build Status](https://github.com/ctu-mrs/bluefox2/workflows/Noetic/badge.svg)](https://github.com/ctu-mrs/bluefox2/actions) |
|--------------|--------------------------------------------------------------------------------------------------------------------------------|

~~*This driver depends on package [`camera_base`](https://github.com/KumarRobotics/camera_base)*~~ it is header-only package - it was moved into the package, so it is not dependecy anymore.

https://github.com/KumarRobotics/bluefox2

![image](http://i.imgur.com/89Bb1hR.png)

[mvBluefox MLC](http://www.matrix-vision.com/USB2.0-single-board-camera-mvbluefox-mlc.html)

## Supported hardware
This driver should work with any Matrix-Vision Bluefox usb2.0 MLC cameras (bluefox2).

Note: Driver binaries were moved to the package and are installed with it, so there is no need to install it separately. If there is a need to update those libraries, you have to manually update driver binary files in the package and commit it. Finding the binaries and loading them during node start/launch is left as responsibility of the ROS2 packaging system.

## Finding camera serial number
Run `ros2 run bluefox2 bluefox2_list_cameras` to list info about all connected cameras. This is designed to be machine readable and is used in the launchfile to automatically select first found camera, so the user does not have to do that manually (although user can specify specific camera if he wants to).

You can also run `ros2 run bluefox2 bluefox2_list_cameras_human_readable` to see more detailes.

## API Stability
The ROS API of this driver should be considered **unstable**.

## ROS API

### Single node

- `single.launch.py` is a node launch file for a single bluefox2 camera.
- Can be included in custom launch files.

### Example

- `example.lauch` is an example of how to include `single_nodelet.launch` in custom launch files. (TODO: check in the ROS1 package version and possibly move it here)

#### Published topics

- `~image_raw` ([sensor_msgs/msg/Image](https://docs.ros.org/en/ros2_packages/jazzy/api/sensor_msgs/msg/Image.html))

    The unprocessed image data.

- `~camera_info` ([sensor_msgs/msg/CameraInfo](https://docs.ros.org/en/ros2_packages/jazzy/api/sensor_msgs/msg/CameraInfo.html))

    Contains the camera calibration (if calibrated) and extra data about the camera configuration.

#### Parameters

**Common interface**

- `~fps` (`integer`)

    The desired frame rate of the camera.

**Node parameters**

- `~identifier` (`string`)

    Serial number of the chosen camera. It has to be explicitly provided as string, otherwise ROS2 api will automatically convert it into an integer, which results in non-matching types error message from the node. Example:

        ros2 run bluefox2 bluefox2_single_node --ros-args --param identifier:="'26808027'" --param frame_id:=mv_26808027

    If you are running it directly, like in this example, you also have to define `frame_id` parameter. Otherwise the `frame_id` in the image message header will be empty. When you run it through launch file, then the frame id will always be set and will have form `mv_<serial>`.

- `~camera_name` (`string`, default: ``)

    Camera name used by `camera_info_manager` for loading calibration file, should be the same as the name in `mv_<serial>.yaml`.

- `~camera` (`string`, default: `<camera_name>`)

    Name of the node.

- `~frame_id` (`string`, default: `<camera>`)

    frame id of the published topics.

- `~calib_url` (`string`)

    camera calibration URL.

**Lauchfile parameteres**

- `camera_namespace` (`string`, default: `UAV_NAME` environment variable)

    Camera namespace used to distinguish topics between different drones (they are going to have different serial numbers anyway, so this is more of a human readability improvement).

    Note: Node namespace will be in the form `/<camera_namespace>/<camera_name>/bluefox2_single`. Specific example: `/uav1/mv_26808027/bluefox2_single`. When running from launchfile, `UAV_NAME` environment variable must be set (otherwise, error is thrown and the node stops).

- `use_camera_name` (`bool`)

    This determines whether `camera_name` will be included in the namespace. Example:

    1. When `use_camera_name` is set to **true**, the entire namespace will be `/uav1/mv_26808027/bluefox2_single`
    2. When `use_camera_name` is set to **false**, the entire namespace will be `/uav1/bluefox2_single`

- `device` (`string`)

    The device serial id. It is initialized either from the environment variable `BLUEFOX` if defined, as the first device discovered by the `bluefox2_list_cameras` utility or manually as an argument to the launchfile.

**Dynamically Reconfigurable Parameters**

This was deleted during migration. We do not expect to need it. If you need it, feel free to implement it.

<!--See the [dynamic_reconfigure](http://wiki.ros.org/dynamic_reconfigure) package for details on dynamically reconfigurable parameters.

white balance parameter:

* `-1` - wbp_unavailable
* `0~5` - wbp_tungsten and friends
* `6` ~ wbp_user1
* `7` - wbp_calibrate, calibrate next frame for white balance

For calibrating white balance, first point the camera at a white board, then select `wbp_calibrate`, the mvIMPACT driver will calibrate white balance automatically and save it to `wbp_user1`.

`~dcfm` (`int`, default: `1`)

dark current filter mode:

* `0` - dcfm_off
* `1` - dcfm_on
* `2` - dcfm_calibrate
* `3` - correction_image

If you are using a color camera, you would want to perform a 'dark current filter calibration` to enhance the quality of acquisition. This is done with the following steps:

1. Start the bluefox node, let it run for about 5-6 minutes until the temperature of the sensor reaches a stable value. Then you would see something like this.
![image](http://i.imgur.com/UzJm5Rv.jpg)
2. Put the lense cap on so that the image looks like this. You can see that there are some pixels that are not completely dark, this is due to the effect of dark current.
![image](http://i.imgur.com/Kk6Rish.jpg)
3. Select `dcfm_calibrate` from the reconfigure server. The driver will do the dark current calibration and then switch the filter on. You can verify the result by selecting `correction_image` and you will see this.
![image](http://i.imgur.com/NC2AZ2l.jpg)
4. Then you can switch back to `dcfm_on` and your image would look much better then before.
![image](http://i.imgur.com/WFrVAOB.jpg)
5. This calibration process cannot be done automatically since it requires the sensor be running for a few minutes and manually putting the lense cap on.

Read this [article](http://www.matrix-vision.com/faq-reader/245.html) as well.

`~hdr` (`bool`, default: `false`)

Only 200wG camera supports this mode, set `hdr` to `true` for other cameras will have no effect.

This mode is required when high fps desired which allows 200wG to work at 90 fps and 200bG at 24 fps (with `ctm = 1`). Using this will result in imprecise time stamp of captured image. Use with caution. -->

## Custom config

You can provide your custom config file like this:

    ros2 launch bluefox2 single.launch.py custom_config:=/path/to/cofig.yaml

Since it is composable node, the namespace of the parameters **is not applied**. Instead of this:

    /uav1/bluefox2_single:
      ros__parameters:
        fps: 25
        g_gain: 0.3
        frame_id: trjkm

it must look just like this, a plain yaml:

    fps: 10
    g_gain: 0.3
    frame_id: trjkm

## Hardware sync

Code for the two cameras was not migrated, because we do not expect to need it. If you need it, feel free to migrate this part.

<!-- Notice that if you are using two 200w cameras, there's no need to use hardware synchronization because software synchronization is supported.

[Using 2 mvBlueFOX-MLC cameras in Master-Slave mode](http://www.matrix-vision.com/manuals/mvBlueFOX/UseCases_page_0.html#UseCases_section_MasterSlave_Mode)

[Single-board version (mvBlueFOX-MLC2xx)](http://www.matrix-vision.com/manuals/mvBlueFOX/mvBF_page_tech.html#mvBF_subsection_single ) -->

## [Install mvIMPACT Driver](http://www.matrix-vision.com/manuals/mvBlueFOX/mvBF_page_quickstart.html#mvBF_subsubsection_quickstart_linux_software)

There is no need to install the driver anymore. All binary library files and header files of the driver were baked directly into the package and are installed together with the package. This was done primarily because we needed to run this driver in the Docker.

Finding and loading those driver's binary library files is left to the ROS2 system. If you need to update those driver's binary library files and header files, you have to manually download and extract newer ones and add them to the package.

There was also `wxPropView` viewer gui application available when installed in original way throught install script. Maybe you can still install it if you need it, but we haven't tested this. We use `rqt_image_view` tool instead.

<!-- Run:

```
./install/install.sh
```

This will install mvIMPACT_Acquire SDK to `/opt`.

## wxPropView
If you install the full matrix vision driver, you will have `wxPropView` installed to your system. It's an GUI application that let you inspect all properties of the camera. -->

## FAQs
1. I have the driver locally in my ros package, but every time I plug in a camera, I need to change the permission.
    * Simple fix:

        ```bash
        sudo chmod 777 /dev/bus/usb/xxx/xxx
        ```

        `/dev/bus/usb/xxx/xxx` can be easily identified with the error information ros provids.

        You can also use convenience script `device_permission_change.bash` in the `scripts` subdirectory, which is essentially doing the same thing. It automatically finds bluefox2 device files by it's vendor id and change their permissions. You must run it with a sudo.

    * Permanent fix:
    Adding a rule to `/etc/udev/rules.d` by the following command

        ```bash
        sudo cp -f path_to_driver/Scripts/51-mvbf.rules /etc/udev/rules.d/
        sudo service udev reload
        ```

        If you want to run the driver in the Docker container, this udev rules installation has to be done on the host system. This is the reason why we created the `device_permission_change.bash` convenience script - you can run it from within the privileged container without the need to set anything on the host system. We emphasize that the **container must be run as privileged for this to work**.

2. Camera acquisition failure after being unplugged and plugged back in
If you are using linux kernel 3.13.0, then it's likely that you will encounter this problem.
The solution is to install the latest kernel, eg. > 3.13

## Running in the docker throught portainer.

You can run the Docker container running this Bluefox2 camera driver conveniently using stack. The most covenient way is to use compose file with inline config. Example:

```yaml
services:
  bluefox2:
    image: ctumrs/bluefox2:unstable
    #build: .
    privileged: true
    network_mode: "host"
    configs:
      - source: cfg_yaml
        target: /custom_config.yaml
        mode: 0444  # read-only permissions
      - source: calibration
        target: /camera_calibration.yaml
        mode: 0444  # read-only permissions
    command: custom_config:=/custom_config.yaml calib_url:=file:///camera_calibration.yaml
    environment:
      - UAV_NAME=uav1
      - USE_CAMERA_NAME=true

configs:

  cfg_yaml:
    content: |
      fps: 25
      #remappings:
      #  '/uav1/image_raw': /abc
      #calib_url: file:///camera_calibration.yaml

  calibration:
    content: |
      image_width: 752
      image_height: 480
      camera_name: mv_26808027
      camera_matrix:
        rows: 3
        cols: 3
        data: [609.952391, 0.000000, 380.176995, 0.000000, 610.025106, 222.719530, 0.000000, 0.000000, 1.000000]
      distortion_model: plumb_bob
      distortion_coefficients:
        rows: 1
        cols: 5
        data: [-2.615388e+02, 0.000000e+00, 9.672868e-04, 4.034114e-06, -1.480877e-08, 2.407404e-11 ]
      rectification_matrix:
        rows: 3
        cols: 3
        data: [1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000]
      projection_matrix:
        rows: 3
        cols: 4
        data: [505.167053, 0.000000, 380.176995, 0.000000, 0.000000, 568.042664, 222.719530, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000]
```

Here you can see the standard definition of the Docker container that will be started using `service` section. There are two important settings:

- `privileged: true`: Must be set to `true` to enable the container to communicate with the Bluefox2 camera usb device.
- `network_mode: "host"`: Must be set to `"host"` to enable the other parts of the system to see the published image topics.

### Configs

You can define configs in one place through inline yaml syntax. Inline configs are defined in the `configs` section. Every config has it's id, like `cfg_yaml` or `calibration`. Instead of the link to file, there is a content of the yaml config file directly. That content is written to the file in the `configs` subsection of the services section. Example:

```yaml
configs:
  - source: cfg_yaml
    target: /custom_config.yaml
    mode: 0444  # read-only permissions
  - source: calibration
    target: /camera_calibration.yaml
    mode: 0444  # read-only permissions
```

Sections containing the inline yaml are refered to by their ids. Their contents are then written to the file defined by `target` subsection. In out case, both files are written directly to the root directoy. Those files are then passed to the container entrypoint which passes them directly to the launch file. This syntax is recommended:

```yaml
command: custom_config:=/custom_config.yaml calib_url:=file:///camera_calibration.yaml
```

In case of the camera calibration, you could also define that file in the general inline yaml config for the launch file:

```yaml
cfg_yaml:
  content: |
    fps: 25
    #remappings:
    #  '/uav1/image_raw': /abc
    #calib_url: file:///camera_calibration.yaml
```

As you can see, you can also remap topics.

Note: You cannot use namespace in the custom config yaml because this node is running as a composable node.