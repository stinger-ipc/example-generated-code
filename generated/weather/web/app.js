const clientId = "weather-web-" + new Date().getTime();
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
        "weatherCondition": [
            {"name": "rainy", "id": 1 },
            
            {"name": "sunny", "id": 2 },
            
            {"name": "partly_cloudy", "id": 3 },
            
            {"name": "mostly_cloudy", "id": 4 },
            
            {"name": "overcast", "id": 5 },
            
            {"name": "windy", "id": 6 },
            
            {"name": "snowy", "id": 7 }
            ]
    };

    $scope.signals = {
        "currentTime": {
            "subscription_id": null,
            "name": "current_time",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/signal/current_time`
        }
    };

    $scope.properties = {
        "location": {
            "subscription_id": null,
            "name": "location",
            "received": { 
                "latitude": null,
            
                "longitude": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/location/value`,
            "update_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/location/update`,
            "response_topic": `client/${client_id}/weather/property/location/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "currentTemperature": {
            "subscription_id": null,
            "name": "current_temperature",
            "received": { 
                "temperature_f": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/current_temperature/value`,
            "flash": false,
            "property_version": -1
        },
    
        "currentCondition": {
            "subscription_id": null,
            "name": "current_condition",
            "received": { 
                "condition": null,
            
                "description": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/current_condition/value`,
            "flash": false,
            "property_version": -1
        },
    
        "dailyForecast": {
            "subscription_id": null,
            "name": "daily_forecast",
            "received": { 
                "monday": { 
                    "high_temperature": null,
                
                    "low_temperature": null,
                
                    "condition": null,
                
                    "start_time": null,
                
                    "end_time": null
                 },
            
                "tuesday": { 
                    "high_temperature": null,
                
                    "low_temperature": null,
                
                    "condition": null,
                
                    "start_time": null,
                
                    "end_time": null
                 },
            
                "wednesday": { 
                    "high_temperature": null,
                
                    "low_temperature": null,
                
                    "condition": null,
                
                    "start_time": null,
                
                    "end_time": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/daily_forecast/value`,
            "flash": false,
            "property_version": -1
        },
    
        "hourlyForecast": {
            "subscription_id": null,
            "name": "hourly_forecast",
            "received": { 
                "hour_0": { 
                    "temperature": null,
                
                    "starttime": null,
                
                    "condition": null
                 },
            
                "hour_1": { 
                    "temperature": null,
                
                    "starttime": null,
                
                    "condition": null
                 },
            
                "hour_2": { 
                    "temperature": null,
                
                    "starttime": null,
                
                    "condition": null
                 },
            
                "hour_3": { 
                    "temperature": null,
                
                    "starttime": null,
                
                    "condition": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/hourly_forecast/value`,
            "flash": false,
            "property_version": -1
        },
    
        "currentConditionRefreshInterval": {
            "subscription_id": null,
            "name": "current_condition_refresh_interval",
            "received": { 
                "seconds": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/current_condition_refresh_interval/value`,
            "update_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/current_condition_refresh_interval/update`,
            "response_topic": `client/${client_id}/weather/property/current_condition_refresh_interval/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "hourlyForecastRefreshInterval": {
            "subscription_id": null,
            "name": "hourly_forecast_refresh_interval",
            "received": { 
                "seconds": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/hourly_forecast_refresh_interval/value`,
            "update_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/hourly_forecast_refresh_interval/update`,
            "response_topic": `client/${client_id}/weather/property/hourly_forecast_refresh_interval/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "dailyForecastRefreshInterval": {
            "subscription_id": null,
            "name": "daily_forecast_refresh_interval",
            "received": { 
                "seconds": null
             },
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/daily_forecast_refresh_interval/value`,
            "update_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/property/daily_forecast_refresh_interval/update`,
            "response_topic": `client/${client_id}/weather/property/daily_forecast_refresh_interval/update/response`,
            "flash": false,
            "property_version": -1
        }
    };

    var interface_name = "weather";
    var client_id = clientId;
    // TODO: support all the topic params

    $scope.methods = {
        "refreshDailyForecast": {
            "name": "refresh_daily_forecast",
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/method/refresh_daily_forecast/request`,
            "response_topic": `client/${client_id}/weather/method/refresh_daily_forecast/response`,
            "pending_correlation_id": null,
            "args": {},
            "received": null,
            "received_time": null
        },
        "refreshHourlyForecast": {
            "name": "refresh_hourly_forecast",
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/method/refresh_hourly_forecast/request`,
            "response_topic": `client/${client_id}/weather/method/refresh_hourly_forecast/response`,
            "pending_correlation_id": null,
            "args": {},
            "received": null,
            "received_time": null
        },
        "refreshCurrentConditions": {
            "name": "refresh_current_conditions",
            "mqtt_topic": `${topicParams.prefix}/weather/${topicParams.service_id}/method/refresh_current_conditions/request`,
            "response_topic": `client/${client_id}/weather/method/refresh_current_conditions/response`,
            "pending_correlation_id": null,
            "args": {},
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

        const methodResponseSubscriptionTopic = `client/${client_id}/weather/method/+/response`;
        var methodResponseSubscriptionId = subscription_count++;
        const methodResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": methodResponseSubscriptionId
            }
        };
        client.subscribe(methodResponseSubscriptionTopic, methodResponseSubOpts);
        console.log("Subscribing to response topic " + methodResponseSubscriptionTopic + " with id " + methodResponseSubscriptionId);
        

        const propertyResponseSubscriptionTopic = `client/${client_id}/weather/property/+/update/response`;
        var propertyResponseSubscriptionId = subscription_count++;
        const propertyResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": propertyResponseSubscriptionId
            }
        };
        client.subscribe(propertyResponseSubscriptionTopic, propertyResponseSubOpts);
        console.log("Subscribing to response topic " + propertyResponseSubscriptionTopic + " with id " + propertyResponseSubscriptionId);

        
        const current_time_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["currentTime"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/weather/${topicParams.service_id}/signal/current_time`);
        client.subscribe(resolvedTopic, current_time_sub_opts);
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