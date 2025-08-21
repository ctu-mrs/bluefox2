#!/usr/bin/env python3

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, OpaqueFunction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, EnvironmentVariable, TextSubstitution, PathJoinSubstitution
from launch_ros.actions import Node
from launch.substitutions import PythonExpression
from launch.actions import LogInfo
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch_ros.substitutions import FindPackageShare
import subprocess
import yaml

def get_available_cameras():
    # You could call your list_cameras executable here
    # For now, this is a placeholder
    try:
        result = subprocess.run(['ros2', 'run', 'bluefox2', 'bluefox2_list_cameras'], 
                              capture_output=True, text=True)
        return result.stdout
    except:
        return []

def generate_launch_description():
    declare_use_camera_name = DeclareLaunchArgument(
        'use_camera_name',
        default_value=EnvironmentVariable('USE_CAMERA_NAME', default_value="false"),
        description='Defines whether the node should use camera name with serial number in it. It may be not very practical when only one camera is used. User has to know camera serial number to use proper namespace when definning parameters.'
    )
    
    declare_custom_config = DeclareLaunchArgument(
        'custom_config',
        default_value='',
        description='config from the user'
    )
    
    # Declare launch arguments
    declare_node_start_delay = DeclareLaunchArgument(
        'node_start_delay',
        default_value='0',
        description='Node delay for multiple cameras (driver can crash if run multiple times in the same moment)'
    )
        
    # Devices listing utility returns camera serial numbers divided by space. The last character is newline, so it is thrown away with '-1' indexing.
    devices = get_available_cameras().split(" ")[0:-1]
    selected_device = ''
    if len(devices):
        selected_device = devices[0]
        devices_search_log = LogInfo(msg=f"Found Bluefox2 devices: {devices}. If user does not select particular device, the device with serial number {selected_device} will be used.")
    else:
        devices_search_log = LogInfo(msg="No Bluefox2 devices found.")
         
    declare_device = DeclareLaunchArgument(
        'device',
        default_value=PythonExpression(['str(', EnvironmentVariable('BLUEFOX', default_value=selected_device), ')']),
        description='Device serial number (can be found by running bluefox2_list_cameras)'
    )
    
    declare_camera_namespace = DeclareLaunchArgument(
        'camera_namespace',
        default_value=EnvironmentVariable('UAV_NAME'),
        description='Camera namespace (used for node name and topic namespace)'
    )
    
    declare_camera_name = DeclareLaunchArgument(
        'camera_name',
        default_value=['mv_', LaunchConfiguration('device')],
        description='Camera name (used for node name and topic namespace)'
    )
    
    declare_camera = DeclareLaunchArgument(
        'camera',
        default_value=LaunchConfiguration('camera_name'),
        description='Camera alias'
    )
    
    declare_frame_id = DeclareLaunchArgument(
        'frame_id',
        default_value=LaunchConfiguration('camera'),
        description='Frame id (used in the header of ROS messages)'
    )
    
    declare_calib_url = DeclareLaunchArgument(
        'calib_url',
        #default_value="",
        # default_value=[
        #     TextSubstitution(text='file://'),
        #     PathJoinSubstitution([
        #         FindPackageShare('mrs_uav_deployment'),
        #         'config/camera_calibrations/mv_',
        #     ]),
        #     LaunchConfiguration('device'),
        #     TextSubstitution(text='.yaml')
        # ],
        default_value=[
            TextSubstitution(text='file://'),
            PathJoinSubstitution([
                FindPackageShare('bluefox2'),
                'config/example_calib.yaml'
            ]),
        ],
        description='The path to the calibration file'
    )
    
    # Camera settings
    declare_fps = DeclareLaunchArgument('fps', default_value='60', description='Frame rate')
    declare_aec = DeclareLaunchArgument('aec', default_value='true', description='Auto exposure control')
    declare_des_grey_value = DeclareLaunchArgument('des_grey_value', default_value='128', description='Desired brightness 0-255 (only when aec == true)')
    declare_expose_upper_limit_us = DeclareLaunchArgument('expose_upper_limit_us', default_value='100000', description='Upper limit of exposure time (only when aec == true)')
    declare_max_expose_jump = DeclareLaunchArgument('max_expose_jump', default_value='1000000', description='Maximal change of exposure time in one step (only when aec == true)')
    declare_acs = DeclareLaunchArgument('acs', default_value='2', description='Auto exposure control speed (0 - slow, 1 - medium, 2 - fast)')
    declare_expose_us = DeclareLaunchArgument('expose_us', default_value='10000', description='Exposure time in microseconds (only when aec == false)')
    declare_agc = DeclareLaunchArgument('agc', default_value='true', description='Auto gain control')
    declare_gain_db = DeclareLaunchArgument('gain_db', default_value='0.0', description='Gain (only when agc == false)')
    declare_wbp = DeclareLaunchArgument('wbp', default_value='3', description='White balance parameter')
    declare_idpf = DeclareLaunchArgument('idpf', default_value='0', description='Pixel format')
    declare_mm = DeclareLaunchArgument('mm', default_value='0', description='Mirror the captured image')
    declare_cbm = DeclareLaunchArgument('cbm', default_value='0', description='Camera binning mode')
    declare_ctm = DeclareLaunchArgument('ctm', default_value='1', description='Camera trigger mode')
    declare_dcfm = DeclareLaunchArgument('dcfm', default_value='0', description='Dark current filter')
    declare_hdr = DeclareLaunchArgument('hdr', default_value='false', description='High dynamic range')
    declare_request = DeclareLaunchArgument('request', default_value='5', description='Request capture queue count')
    
    # Compression settings
    declare_compressed_jpeg_quality = DeclareLaunchArgument('compressed_jpeg_quality', default_value='90')
    declare_theora_keyframe_frequency = DeclareLaunchArgument('theora_keyframe_frequency', default_value='60')
    declare_theora_target_bitrate = DeclareLaunchArgument('theora_target_bitrate', default_value='50000')
    declare_theora_quality = DeclareLaunchArgument('theora_quality', default_value='8')
    declare_theora_optimize_for = DeclareLaunchArgument('theora_optimize_for', default_value='0')
    
    # Node settings
    declare_output = DeclareLaunchArgument('output', default_value='screen', description='Text output to screen/log')
    declare_rectify = DeclareLaunchArgument('rectify', default_value='true', description='Run rectification')
    declare_view = DeclareLaunchArgument('view', default_value='false', description='Run camera viewer')
    declare_calib = DeclareLaunchArgument('calib', default_value='false', description='Run calibration')
    
    # Calibration settings
    declare_size = DeclareLaunchArgument('size', default_value='8x6', description='Calibration pattern size')
    declare_square = DeclareLaunchArgument('square', default_value='0.108', description='Calibration square size')
    declare_image = DeclareLaunchArgument('image', default_value='image_raw', description='Image topic for viewer')
    
    # Environment setup for custom libusb
    env_vars = {
        'LD_LIBRARY_PATH': '/opt/mvIMPACT_acquire_libusb:' + os.environ.get('LD_LIBRARY_PATH', '')
    }
    
    # ========================================================================================================
    # TODO: this has to be reworked somwhow - composable nodes does not use namespace for the params while classical node
    # do use namespace, so we would have to parse the yaml twiceand have there parameters defined twice - very messy.
    # We would probably add that as some new python module.
    #
    # This is done like this because, the 'identifier' parameters is taken from the environment variable as a
    # string containing only numbers. ROS2 launch system is always trying to convertit into a integer. So it
    # is gibing the node integer, although itshould really be a string. We made a workaround here - creating a
    # Node throught opaque function, which gives us a 'context' object used to extract raw string from the
    # LaunchConfiguration object (see 'identifier' param).
    def get_processed_launch_objects(context):
        _custom_config_file = LaunchConfiguration('custom_config').perform(context)
        if LaunchConfiguration("use_camera_name").perform(context) == "true":
            prefix = f"/{LaunchConfiguration('camera_namespace').perform(context)}/{LaunchConfiguration('camera_name').perform(context)}/bluefox2_single"
        else:
            prefix = f"/{LaunchConfiguration('camera_namespace').perform(context)}/bluefox2_single"
        remappings = []
        
        # pull remapping of the topics out of the yaml file
        if _custom_config_file != '':
            with open(_custom_config_file, 'r') as f:
                yaml_data = yaml.load(f, Loader=yaml.FullLoader)

                if prefix in yaml_data and 'remappings' in yaml_data[prefix]['ros__parameters']:
                    remappings_subyaml = yaml_data[prefix]['ros__parameters']['remappings']
                    for orig_name in remappings_subyaml:
                        new_name = remappings_subyaml[orig_name]
                        #print(f"remapping topic {orig_name} to {new_name}")
                        remappings.append((orig_name, new_name))

        # Real prefix, without the "bluefox2_single" at the end ("bluefox2_single" that will be appended by the node itself).
        if LaunchConfiguration("use_camera_name").perform(context) == "true":
            real_prefix = f"/{LaunchConfiguration('camera_namespace').perform(context)}/{LaunchConfiguration('camera_name').perform(context)}"
        else:
            real_prefix = f"/{LaunchConfiguration('camera_namespace').perform(context)}"
        
        objects = [
            LogInfo(msg=f"custom config file: {_custom_config_file}"),
            LogInfo(msg=f"remappings:"),
        ]
        
        for remapping in remappings:
            objects.append(LogInfo(msg=f"\t{remapping[0]} -> {remapping[1]}"))
            
        parameters = [{
            'identifier': LaunchConfiguration('device').perform(context),
            'frame_id': LaunchConfiguration('frame_id'),
            'camera_name': LaunchConfiguration('camera_name'),
            'calib_url': LaunchConfiguration('calib_url'),
            'fps': LaunchConfiguration('fps'),
            'idpf': LaunchConfiguration('idpf'),
            'aec': LaunchConfiguration('aec'),
            'expose_us': LaunchConfiguration('expose_us'),
            'agc': LaunchConfiguration('agc'),  # Fixed the typo from 'aec'
            'gain_db': LaunchConfiguration('gain_db'),
            'cbm': LaunchConfiguration('cbm'),
            'ctm': LaunchConfiguration('ctm'),
            'dcfm': LaunchConfiguration('dcfm'),
            'hdr': LaunchConfiguration('hdr'),
            'wbp': LaunchConfiguration('wbp'),
            'request': LaunchConfiguration('request'),
            'mm': LaunchConfiguration('mm'),
            'expose_upper_limit_us': LaunchConfiguration('expose_upper_limit_us'),
            'max_expose_jump': LaunchConfiguration('max_expose_jump'),
            'des_grey_value': LaunchConfiguration('des_grey_value'),
            'acs': LaunchConfiguration('acs'),
            'image_raw/compressed/jpeg_quality': LaunchConfiguration('compressed_jpeg_quality'),
            'image_raw/theora/keyframe_frequency': LaunchConfiguration('theora_keyframe_frequency'),
            'image_raw/theora/target_bitrate': LaunchConfiguration('theora_target_bitrate'),
            'image_raw/theora/quality': LaunchConfiguration('theora_quality'),
            'image_raw/theora/optimize_for': LaunchConfiguration('theora_optimize_for'),
        }]
        
        if _custom_config_file != '':
            print("appending params")
            parameters.append(_custom_config_file)
            
        objects.append(
            ComposableNodeContainer(
                name='bluefox2_container',
                namespace='',
                package='rclcpp_components',
                executable='component_container',
                output=LaunchConfiguration('output'),
                respawn=False,
                additional_env=env_vars,
                #prefix='xterm -e gdb -ex run --args',
                composable_node_descriptions=[
                    ComposableNode(
                        package='bluefox2',
                        plugin='bluefox2::BluefoxSingleComponent',  # Assuming the nodelet is converted to a regular node
                        #name=LaunchConfiguration('camera'),    # This is commented out because with it, the node name looked like "/uav1/mv_26808027/mv_26808027". Without it it looks like "/uav1/mv_26808027/bluefox2_single".
                        namespace=real_prefix,
                        parameters=parameters,
                        remappings=remappings,
                        extra_arguments=[{'use_intra_process_comms': True}],
                    ),
                ]
            )
        )
        
        rectify_remappings=[
            ('image', real_prefix + '/image_raw'),
            ('camera_info', real_prefix + '/camera_info')
        ]
        
        rectify_remappings += remappings
        
        # new_rectify_remappings = []
        
        # It looks like this is not needed :) You just append the remappings of the driver node and
        # the rectify node figures out by itself all the remapping substitutions. Nodes are quite clever :D
        # Leaving here the old way just in case.
        
        # # We also have to check the remappings of the driver node beause the rectify node is dependent on them.
        # # If we remap only the topics of the camera node and leave the rectify node topics as they are, then
        # # the rectify node would subscribe wront topics.
        # for orig_remap_rule in rectify_remappings:
        #     orig_remap = orig_remap_rule[1]
        #     for dependent_remap in remappings:
        #         orig_topic = dependent_remap[0]
        #         if orig_remap == orig_topic:
        #             new_remap = dependent_remap[1]
        #             new_remap_rule = (orig_remap_rule[0], new_remap)
        #             new_rectify_remappings.append(new_remap_rule)
        #         else:
        #             new_rectify_remappings.append(orig_remap_rule)
                    
        print("rectify_remappings:\n", rectify_remappings)
        # print("new_rectify_remappings:\n", new_rectify_remappings)
        
        objects.append(
            Node(
                package='image_proc',
                executable='rectify_node',
                name='rectify_mono',
                namespace=real_prefix,
                condition=IfCondition(LaunchConfiguration('rectify')),
                # remappings=new_rectify_remappings
                remappings=rectify_remappings
            )
        )
        
        return objects
    
    bluefox2_node = OpaqueFunction(function=get_processed_launch_objects)
    
    # Camera viewer node
    viewer_node = Node(
        package='image_view',
        executable='image_view',
        name='viewer',
        namespace=LaunchConfiguration('camera'),
        condition=IfCondition(LaunchConfiguration('view')),
        output=LaunchConfiguration('output'),
        arguments=[PythonExpression(['image:=', LaunchConfiguration('image')])]
    )
    
    # Camera calibration node
    calibration_group = GroupAction(
        condition=IfCondition(LaunchConfiguration('calib')),
        actions=[
            DeclareLaunchArgument('pattern', default_value='chessboard'),
            DeclareLaunchArgument('num_dist_coeff', default_value='2'),
            Node(
                package='camera_calibration',
                executable='cameracalibrator',
                name='calibrator',
                output='screen',
                arguments=[
                    '-p', LaunchConfiguration('pattern'),
                    '-s', LaunchConfiguration('size'),
                    '-q', LaunchConfiguration('square'),
                    '-k', LaunchConfiguration('num_dist_coeff'),
                    PythonExpression(['image:=/', LaunchConfiguration('camera'), '/image_raw']),
                    PythonExpression(['camera:=/', LaunchConfiguration('camera')])
                ]
            )
        ]
    )
    
    return LaunchDescription([
        # Declare all arguments
        declare_use_camera_name,
        devices_search_log,
        declare_custom_config,
        declare_node_start_delay,
        declare_device,
        declare_camera_namespace,
        declare_camera_name,
        declare_camera,
        declare_frame_id,
        declare_calib_url,
        declare_fps,
        declare_aec,
        declare_des_grey_value,
        declare_expose_upper_limit_us,
        declare_max_expose_jump,
        declare_acs,
        declare_expose_us,
        declare_agc,
        declare_gain_db,
        declare_wbp,
        declare_idpf,
        declare_mm,
        declare_cbm,
        declare_ctm,
        declare_dcfm,
        declare_hdr,
        declare_request,
        declare_compressed_jpeg_quality,
        declare_theora_keyframe_frequency,
        declare_theora_target_bitrate,
        declare_theora_quality,
        declare_theora_optimize_for,
        declare_output,
        declare_rectify,
        declare_view,
        declare_calib,
        declare_size,
        declare_square,
        declare_image,
        
        # Launch nodes
        bluefox2_node,
        viewer_node,
        calibration_group,
    ])