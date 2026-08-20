const clientId = "testable-web-" + new Date().getTime();
const client_id = clientId;

var responseSubscriptionId = null;

const topicParams = Object.fromEntries(new URLSearchParams(location.search));

const instanceId = topicParams.instance_id || null;

function makeRequestProperties() {
    const correlationData = Math.random().toString(16).substr(2, 8);
    return {
        "contentType": "application/json",
        "correlationData": correlationData
    }
}

// Replace '+' tokens in topics with the instance id (if present)
function resolveTopic(topic) {
    if (!instanceId) return topic;
    return topic.replace(/\+/g, instanceId);
}

var app = angular.module("myApp", []);

app.controller("myCtrl", function ($scope, $filter, $location, $timeout) {

    console.log("Running app");

    var subscription_state = 0;

    // Briefly flash a component's title bar to indicate a freshly received value.
    function triggerFlash(obj) {
        obj.flash = false;
        $timeout(function () {
            obj.flash = true;
            $timeout(function () {
                obj.flash = false;
            }, 800);
        }, 0);
    }

    $scope.timePattern = new RegExp("^[0-2][0-9]:[0-5][0-9]$");
    $scope.online = false;

    // Active tab — default to the first tab that has content
    
    $scope.activeTab = 'signals';
    

    $scope.enums = {
        "numbers": [
            {"name": "one", "id": 1 },
            
            {"name": "two", "id": 2 },
            
            {"name": "three", "id": 3 }
            ]
    };

    $scope.signals = {
        "empty": {
            "subscription_id": null,
            "name": "empty",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/empty`
        },
    
        "singleInt": {
            "subscription_id": null,
            "name": "singleInt",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleInt`
        },
    
        "jsonSchemaValidatedInt": {
            "subscription_id": null,
            "name": "jsonSchemaValidatedInt",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/jsonSchemaValidatedInt`
        },
    
        "singleOptionalInt": {
            "subscription_id": null,
            "name": "singleOptionalInt",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalInt`
        },
    
        "threeIntegers": {
            "subscription_id": null,
            "name": "threeIntegers",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeIntegers`
        },
    
        "singleString": {
            "subscription_id": null,
            "name": "singleString",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleString`
        },
    
        "jsonSchemaValidatedString": {
            "subscription_id": null,
            "name": "jsonSchemaValidatedString",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/jsonSchemaValidatedString`
        },
    
        "singleOptionalString": {
            "subscription_id": null,
            "name": "singleOptionalString",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalString`
        },
    
        "threeStrings": {
            "subscription_id": null,
            "name": "threeStrings",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeStrings`
        },
    
        "singleEnum": {
            "subscription_id": null,
            "name": "singleEnum",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleEnum`
        },
    
        "singleOptionalEnum": {
            "subscription_id": null,
            "name": "singleOptionalEnum",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalEnum`
        },
    
        "threeEnums": {
            "subscription_id": null,
            "name": "threeEnums",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeEnums`
        },
    
        "singleStruct": {
            "subscription_id": null,
            "name": "singleStruct",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleStruct`
        },
    
        "singleOptionalStruct": {
            "subscription_id": null,
            "name": "singleOptionalStruct",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalStruct`
        },
    
        "threeStructs": {
            "subscription_id": null,
            "name": "threeStructs",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeStructs`
        },
    
        "singleDateTime": {
            "subscription_id": null,
            "name": "singleDateTime",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleDateTime`
        },
    
        "singleOptionalDatetime": {
            "subscription_id": null,
            "name": "singleOptionalDatetime",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalDatetime`
        },
    
        "threeDateTimes": {
            "subscription_id": null,
            "name": "threeDateTimes",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeDateTimes`
        },
    
        "singleDuration": {
            "subscription_id": null,
            "name": "singleDuration",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleDuration`
        },
    
        "singleOptionalDuration": {
            "subscription_id": null,
            "name": "singleOptionalDuration",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalDuration`
        },
    
        "threeDurations": {
            "subscription_id": null,
            "name": "threeDurations",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeDurations`
        },
    
        "singleBinary": {
            "subscription_id": null,
            "name": "singleBinary",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleBinary`
        },
    
        "singleOptionalBinary": {
            "subscription_id": null,
            "name": "singleOptionalBinary",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalBinary`
        },
    
        "threeBinaries": {
            "subscription_id": null,
            "name": "threeBinaries",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeBinaries`
        },
    
        "singleArrayOfIntegers": {
            "subscription_id": null,
            "name": "singleArrayOfIntegers",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleArrayOfIntegers`
        },
    
        "singleOptionalArrayOfStrings": {
            "subscription_id": null,
            "name": "singleOptionalArrayOfStrings",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalArrayOfStrings`
        },
    
        "arrayOfEveryType": {
            "subscription_id": null,
            "name": "arrayOfEveryType",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/signal/arrayOfEveryType`
        }
    };

    $scope.properties = {
        "readWriteInteger": {
            "subscription_id": null,
            "name": "read_write_integer",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_integer/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_integer/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_integer/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readOnlyInteger": {
            "subscription_id": null,
            "name": "read_only_integer",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_only_integer/value`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalInteger": {
            "subscription_id": null,
            "name": "read_write_optional_integer",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_integer/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_integer/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_integer/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoIntegers": {
            "subscription_id": null,
            "name": "read_write_two_integers",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_integers/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_integers/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_integers/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readOnlyString": {
            "subscription_id": null,
            "name": "read_only_string",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_only_string/value`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteString": {
            "subscription_id": null,
            "name": "read_write_string",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_string/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_string/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_string/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalString": {
            "subscription_id": null,
            "name": "read_write_optional_string",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_string/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_string/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_string/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoStrings": {
            "subscription_id": null,
            "name": "read_write_two_strings",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_strings/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_strings/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_strings/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteStruct": {
            "subscription_id": null,
            "name": "read_write_struct",
            "received": { 
                "value": { 
                    "the_bool": null,
                
                    "the_int": null,
                
                    "the_number": null,
                
                    "the_str": null,
                
                    "the_enum": null,
                
                    "an_entry_object": null,
                
                    "date_and_time": null,
                
                    "time_duration": null,
                
                    "data": null,
                
                    "OptionalInteger": null,
                
                    "OptionalString": null,
                
                    "OptionalEnum": null,
                
                    "optionalEntryObject": null,
                
                    "OptionalDateTime": null,
                
                    "OptionalDuration": null,
                
                    "OptionalBinary": null,
                
                    "array_of_integers": null,
                
                    "optional_array_of_integers": null,
                
                    "array_of_strings": null,
                
                    "optional_array_of_strings": null,
                
                    "array_of_enums": null,
                
                    "optional_array_of_enums": null,
                
                    "array_of_datetimes": null,
                
                    "optional_array_of_datetimes": null,
                
                    "array_of_durations": null,
                
                    "optional_array_of_durations": null,
                
                    "array_of_binaries": null,
                
                    "optional_array_of_binaries": null,
                
                    "array_of_entry_objects": null,
                
                    "optional_array_of_entry_objects": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_struct/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_struct/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_struct/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalStruct": {
            "subscription_id": null,
            "name": "read_write_optional_struct",
            "received": { 
                "value": { 
                    "the_bool": null,
                
                    "the_int": null,
                
                    "the_number": null,
                
                    "the_str": null,
                
                    "the_enum": null,
                
                    "an_entry_object": null,
                
                    "date_and_time": null,
                
                    "time_duration": null,
                
                    "data": null,
                
                    "OptionalInteger": null,
                
                    "OptionalString": null,
                
                    "OptionalEnum": null,
                
                    "optionalEntryObject": null,
                
                    "OptionalDateTime": null,
                
                    "OptionalDuration": null,
                
                    "OptionalBinary": null,
                
                    "array_of_integers": null,
                
                    "optional_array_of_integers": null,
                
                    "array_of_strings": null,
                
                    "optional_array_of_strings": null,
                
                    "array_of_enums": null,
                
                    "optional_array_of_enums": null,
                
                    "array_of_datetimes": null,
                
                    "optional_array_of_datetimes": null,
                
                    "array_of_durations": null,
                
                    "optional_array_of_durations": null,
                
                    "array_of_binaries": null,
                
                    "optional_array_of_binaries": null,
                
                    "array_of_entry_objects": null,
                
                    "optional_array_of_entry_objects": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_struct/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_struct/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_struct/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoStructs": {
            "subscription_id": null,
            "name": "read_write_two_structs",
            "received": { 
                "first": { 
                    "the_bool": null,
                
                    "the_int": null,
                
                    "the_number": null,
                
                    "the_str": null,
                
                    "the_enum": null,
                
                    "an_entry_object": null,
                
                    "date_and_time": null,
                
                    "time_duration": null,
                
                    "data": null,
                
                    "OptionalInteger": null,
                
                    "OptionalString": null,
                
                    "OptionalEnum": null,
                
                    "optionalEntryObject": null,
                
                    "OptionalDateTime": null,
                
                    "OptionalDuration": null,
                
                    "OptionalBinary": null,
                
                    "array_of_integers": null,
                
                    "optional_array_of_integers": null,
                
                    "array_of_strings": null,
                
                    "optional_array_of_strings": null,
                
                    "array_of_enums": null,
                
                    "optional_array_of_enums": null,
                
                    "array_of_datetimes": null,
                
                    "optional_array_of_datetimes": null,
                
                    "array_of_durations": null,
                
                    "optional_array_of_durations": null,
                
                    "array_of_binaries": null,
                
                    "optional_array_of_binaries": null,
                
                    "array_of_entry_objects": null,
                
                    "optional_array_of_entry_objects": null
                 },
            
                "second": { 
                    "the_bool": null,
                
                    "the_int": null,
                
                    "the_number": null,
                
                    "the_str": null,
                
                    "the_enum": null,
                
                    "an_entry_object": null,
                
                    "date_and_time": null,
                
                    "time_duration": null,
                
                    "data": null,
                
                    "OptionalInteger": null,
                
                    "OptionalString": null,
                
                    "OptionalEnum": null,
                
                    "optionalEntryObject": null,
                
                    "OptionalDateTime": null,
                
                    "OptionalDuration": null,
                
                    "OptionalBinary": null,
                
                    "array_of_integers": null,
                
                    "optional_array_of_integers": null,
                
                    "array_of_strings": null,
                
                    "optional_array_of_strings": null,
                
                    "array_of_enums": null,
                
                    "optional_array_of_enums": null,
                
                    "array_of_datetimes": null,
                
                    "optional_array_of_datetimes": null,
                
                    "array_of_durations": null,
                
                    "optional_array_of_durations": null,
                
                    "array_of_binaries": null,
                
                    "optional_array_of_binaries": null,
                
                    "array_of_entry_objects": null,
                
                    "optional_array_of_entry_objects": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_structs/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_structs/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_structs/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readOnlyEnum": {
            "subscription_id": null,
            "name": "read_only_enum",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_only_enum/value`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteEnum": {
            "subscription_id": null,
            "name": "read_write_enum",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_enum/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_enum/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_enum/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalEnum": {
            "subscription_id": null,
            "name": "read_write_optional_enum",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_enum/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_enum/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_enum/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoEnums": {
            "subscription_id": null,
            "name": "read_write_two_enums",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_enums/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_enums/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_enums/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteDatetime": {
            "subscription_id": null,
            "name": "read_write_datetime",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_datetime/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_datetime/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_datetime/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalDatetime": {
            "subscription_id": null,
            "name": "read_write_optional_datetime",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_datetime/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_datetime/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_datetime/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoDatetimes": {
            "subscription_id": null,
            "name": "read_write_two_datetimes",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_datetimes/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_datetimes/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_datetimes/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteDuration": {
            "subscription_id": null,
            "name": "read_write_duration",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_duration/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_duration/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_duration/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalDuration": {
            "subscription_id": null,
            "name": "read_write_optional_duration",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_duration/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_duration/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_duration/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoDurations": {
            "subscription_id": null,
            "name": "read_write_two_durations",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_durations/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_durations/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_durations/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteBinary": {
            "subscription_id": null,
            "name": "read_write_binary",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_binary/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_binary/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_binary/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteOptionalBinary": {
            "subscription_id": null,
            "name": "read_write_optional_binary",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_binary/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_optional_binary/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_optional_binary/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteTwoBinaries": {
            "subscription_id": null,
            "name": "read_write_two_binaries",
            "received": { 
                "first": null,
            
                "second": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_binaries/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_two_binaries/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_two_binaries/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteListOfStrings": {
            "subscription_id": null,
            "name": "read_write_list_of_strings",
            "received": { 
                "value": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_list_of_strings/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_list_of_strings/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_list_of_strings/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "readWriteLists": {
            "subscription_id": null,
            "name": "read_write_lists",
            "received": { 
                "the_list": null,
            
                "optionalList": null
             },
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_lists/value`,
            "update_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/property/read_write_lists/update`,
            "response_topic": `client/${client_id}/testable/property/read_write_lists/update/response`,
            "flash": false,
            "property_version": -1
        }
    };

    var interface_name = "testable";
    var client_id = clientId;
    // TODO: support all the topic params

    $scope.methods = {
        "callWithNothing": {
            "name": "callWithNothing",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callWithNothing/request`,
            "response_topic": `client/${client_id}/testable/method/callWithNothing/response`,
            "pending_correlation_id": null,
            "args": {},
            "received": null,
            "received_time": null
        },
        "callOneInteger": {
            "name": "callOneInteger",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneInteger/request`,
            "response_topic": `client/${client_id}/testable/method/callOneInteger/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalInteger": {
            "name": "callOptionalInteger",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalInteger/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalInteger/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeIntegers": {
            "name": "callThreeIntegers",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeIntegers/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeIntegers/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneString": {
            "name": "callOneString",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneString/request`,
            "response_topic": `client/${client_id}/testable/method/callOneString/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalString": {
            "name": "callOptionalString",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalString/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalString/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeStrings": {
            "name": "callThreeStrings",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeStrings/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeStrings/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneEnum": {
            "name": "callOneEnum",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneEnum/request`,
            "response_topic": `client/${client_id}/testable/method/callOneEnum/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalEnum": {
            "name": "callOptionalEnum",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalEnum/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalEnum/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeEnums": {
            "name": "callThreeEnums",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeEnums/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeEnums/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneStruct": {
            "name": "callOneStruct",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneStruct/request`,
            "response_topic": `client/${client_id}/testable/method/callOneStruct/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalStruct": {
            "name": "callOptionalStruct",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalStruct/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalStruct/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeStructs": {
            "name": "callThreeStructs",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeStructs/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeStructs/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneDateTime": {
            "name": "callOneDateTime",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneDateTime/request`,
            "response_topic": `client/${client_id}/testable/method/callOneDateTime/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalDateTime": {
            "name": "callOptionalDateTime",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalDateTime/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalDateTime/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeDateTimes": {
            "name": "callThreeDateTimes",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeDateTimes/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeDateTimes/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneDuration": {
            "name": "callOneDuration",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneDuration/request`,
            "response_topic": `client/${client_id}/testable/method/callOneDuration/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalDuration": {
            "name": "callOptionalDuration",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalDuration/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalDuration/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeDurations": {
            "name": "callThreeDurations",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeDurations/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeDurations/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneBinary": {
            "name": "callOneBinary",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneBinary/request`,
            "response_topic": `client/${client_id}/testable/method/callOneBinary/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalBinary": {
            "name": "callOptionalBinary",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalBinary/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalBinary/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callThreeBinaries": {
            "name": "callThreeBinaries",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callThreeBinaries/request`,
            "response_topic": `client/${client_id}/testable/method/callThreeBinaries/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null,
            
                "input3": null
            },
            "received": null,
            "received_time": null
        },
        "callOneListOfIntegers": {
            "name": "callOneListOfIntegers",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOneListOfIntegers/request`,
            "response_topic": `client/${client_id}/testable/method/callOneListOfIntegers/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callOptionalListOfFloats": {
            "name": "callOptionalListOfFloats",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callOptionalListOfFloats/request`,
            "response_topic": `client/${client_id}/testable/method/callOptionalListOfFloats/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null
            },
            "received": null,
            "received_time": null
        },
        "callTwoLists": {
            "name": "callTwoLists",
            "mqtt_topic": `${topicParams.prefix}/testable/${topicParams.service_id}/method/callTwoLists/request`,
            "response_topic": `client/${client_id}/testable/method/callTwoLists/response`,
            "pending_correlation_id": null,
            "args": {
                "input1": null,
            
                "input2": null
            },
            "received": null,
            "received_time": null
        }
    };

    $scope.console = {
        showing: false,
        requests: []
    }

    $scope.showoutput = false;
    $scope.linuxoutput = '';

    const brokerUrl = 'ws://' + location.hostname + ':' + location.port + '/ws'

    const connectOptions = {
        keepAlive: 60,
        clientId: clientId,
        protocolId: 'MQTT',
        protocolVersion: 5,
        clean: true
    };

    var client = mqtt.connect(brokerUrl, connectOptions);

    client.on('close', function() {
        console.log("Connection Lost");
    });

    function publish_method_request(method, payload) {
        var qos = 1;
        console.log("METHOD REQUEST", method, payload);
        let props = makeRequestProperties();
        props.responseTopic = method.response_topic;
        $scope.console.requests.unshift({"name":method.name, "correlationData":props.correlationData, "topic": method.mqtt_topic, "payload": payload, "response": null, "requestTime": Date.now()});
        console.log("PUBLISH REQUEST", method.mqtt_topic, payload, props);
        client.publish(method.mqtt_topic, payload, { "qos": qos, retain: false, properties: props});
        return props.correlationData;
    }

    function publish_property_update(prop_obj, payload) {
        console.log("PROPERTY UPDATE", prop_obj, payload);
        const correlationData = Math.random().toString(16).substr(2, 8);
        let props = {
            "contentType": "application/json",
            "userProperties": {
                "PropertyVersion": prop_obj.property_version
            },
            "correlationData": correlationData,
            "responseTopic": prop_obj.response_topic
        };
        $scope.console.requests.unshift({"name":prop_obj.name, "correlationData":correlationData, "topic": prop_obj.update_topic, "payload": payload, "response": null, "requestTime": Date.now()});
        console.log("PUBLISH UPDATE", prop_obj.update_topic, payload, props);
        client.publish(prop_obj.update_topic, payload, { "qos": 1, retain: false, properties: props});
        return;
    }

    client.on('message', function(topic, message, packet) {
        
        const subid = packet.properties.subscriptionIdentifier;

        console.log("Message Arrived: " + topic + " (" + subid + ")");

        var obj;
        if (message.toString().length == 0) {
            obj = null;
        } else {
            obj = JSON.parse(message.toString());
        }
        console.log(obj);

        for (const key in $scope.signals) {
            if (!$scope.signals.hasOwnProperty(key)) continue;
            const sig = $scope.signals[key];
            if (sig.subscription_id == subid) {
                sig.received = obj;
                sig.received_time = new Date();
                triggerFlash(sig);
            }
        }
        for (const key in $scope.properties) {
            if (!$scope.properties.hasOwnProperty(key)) continue;
            const prop = $scope.properties[key];
            if (prop.subscription_id == subid) {
                prop.received = obj;
                console.log("Set property '" + prop.name + "' received object to ", prop.received);
                prop.property_version = packet.properties.userProperties.PropertyVersion;
                triggerFlash(prop);
            }
        }
        for (const key in $scope.methods) {
            if (!$scope.methods.hasOwnProperty(key)) continue;
            const method = $scope.methods[key];
            if (responseSubscriptionId == subid) {
                if (packet.properties.correlationData && method.pending_correlation_id == packet.properties.correlationData) {
                    method.received = obj;
                    method.received_time = new Date();
                    for (let i=0; i<$scope.console.requests.length; i++) {
                        const req = $scope.console.requests[i];
                        if (req.correlationData == packet.properties.correlationData) {
                            req.response = obj;
                            req.responseTime = Date.now();
                        }
                    }
                }
            }
        }

        $scope.$apply();
    });

    client.on('connect', function() {
        $scope.online = true;

        var subscription_count = 10;
        console.log("Connected with ", client);

        const methodResponseSubscriptionTopic = `client/${client_id}/testable/method/+/response`;
        var methodResponseSubscriptionId = subscription_count++;
        const methodResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": methodResponseSubscriptionId
            }
        };
        client.subscribe(methodResponseSubscriptionTopic, methodResponseSubOpts);
        console.log("Subscribing to response topic " + methodResponseSubscriptionTopic + " with id " + methodResponseSubscriptionId);
        

        const propertyResponseSubscriptionTopic = `client/${client_id}/testable/property/+/update/response`;
        var propertyResponseSubscriptionId = subscription_count++;
        const propertyResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": propertyResponseSubscriptionId
            }
        };
        client.subscribe(propertyResponseSubscriptionTopic, propertyResponseSubOpts);
        console.log("Subscribing to response topic " + propertyResponseSubscriptionTopic + " with id " + propertyResponseSubscriptionId);

        
        const empty_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["empty"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/empty`);
        client.subscribe(resolvedTopic, empty_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_int_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleInt"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleInt`);
        client.subscribe(resolvedTopic, single_int_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const json_schema_validated_int_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["jsonSchemaValidatedInt"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/jsonSchemaValidatedInt`);
        client.subscribe(resolvedTopic, json_schema_validated_int_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_int_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalInt"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalInt`);
        client.subscribe(resolvedTopic, single_optional_int_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_integers_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeIntegers"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeIntegers`);
        client.subscribe(resolvedTopic, three_integers_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_string_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleString"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleString`);
        client.subscribe(resolvedTopic, single_string_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const json_schema_validated_string_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["jsonSchemaValidatedString"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/jsonSchemaValidatedString`);
        client.subscribe(resolvedTopic, json_schema_validated_string_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_string_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalString"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalString`);
        client.subscribe(resolvedTopic, single_optional_string_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_strings_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeStrings"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeStrings`);
        client.subscribe(resolvedTopic, three_strings_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_enum_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleEnum"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleEnum`);
        client.subscribe(resolvedTopic, single_enum_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_enum_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalEnum"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalEnum`);
        client.subscribe(resolvedTopic, single_optional_enum_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_enums_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeEnums"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeEnums`);
        client.subscribe(resolvedTopic, three_enums_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_struct_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleStruct"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleStruct`);
        client.subscribe(resolvedTopic, single_struct_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_struct_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalStruct"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalStruct`);
        client.subscribe(resolvedTopic, single_optional_struct_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_structs_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeStructs"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeStructs`);
        client.subscribe(resolvedTopic, three_structs_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_date_time_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleDateTime"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleDateTime`);
        client.subscribe(resolvedTopic, single_date_time_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_datetime_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalDatetime"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalDatetime`);
        client.subscribe(resolvedTopic, single_optional_datetime_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_date_times_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeDateTimes"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeDateTimes`);
        client.subscribe(resolvedTopic, three_date_times_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_duration_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleDuration"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleDuration`);
        client.subscribe(resolvedTopic, single_duration_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_duration_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalDuration"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalDuration`);
        client.subscribe(resolvedTopic, single_optional_duration_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_durations_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeDurations"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeDurations`);
        client.subscribe(resolvedTopic, three_durations_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_binary_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleBinary"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleBinary`);
        client.subscribe(resolvedTopic, single_binary_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_binary_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalBinary"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalBinary`);
        client.subscribe(resolvedTopic, single_optional_binary_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const three_binaries_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["threeBinaries"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/threeBinaries`);
        client.subscribe(resolvedTopic, three_binaries_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_array_of_integers_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleArrayOfIntegers"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleArrayOfIntegers`);
        client.subscribe(resolvedTopic, single_array_of_integers_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const single_optional_array_of_strings_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["singleOptionalArrayOfStrings"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/singleOptionalArrayOfStrings`);
        client.subscribe(resolvedTopic, single_optional_array_of_strings_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const array_of_every_type_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["arrayOfEveryType"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/testable/${topicParams.service_id}/signal/arrayOfEveryType`);
        client.subscribe(resolvedTopic, array_of_every_type_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        

        for (const key in $scope.properties) {
            if (!$scope.properties.hasOwnProperty(key)) continue;
            var sub_id = subscription_count++;
            const prop_sub_opts = {
                "qos": 1,
                "properties": {
                    "subscriptionIdentifier": sub_id
                }
            };
            $scope.properties[key].subscription_id = sub_id;
            var resolvedTopic = resolveTopic($scope.properties[key].mqtt_topic);
            client.subscribe(resolvedTopic, prop_sub_opts);
            console.log("Subscribing to property " + resolvedTopic + " with id " + $scope.properties[key].subscription_id);
        }

        subscription_state = 1;
        $scope.$apply();
    });

    $scope.updateProperty = function(prop) {
        const payload = JSON.stringify(prop.received);
        publish_property_update(prop, payload);
    };
 
    $scope.callMethod = function(method) {
        const payload = {};
        for (const key in method.args) {
            if (!method.args.hasOwnProperty(key)) continue;
            payload[key] = method.args[key];
        }
        const payload_str = JSON.stringify(payload);
        method.pending_correlation_id = publish_method_request(method, payload_str);
    };
});