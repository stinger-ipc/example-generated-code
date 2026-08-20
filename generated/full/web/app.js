const clientId = "Full-web-" + new Date().getTime();
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
        "dayOfTheWeek": [
            {"name": "Sunday", "id": 1 },
            
            {"name": "Monday", "id": 2 },
            
            {"name": "Tuesday", "id": 3 },
            
            {"name": "Wednesday", "id": 4 },
            
            {"name": "Thursday", "id": 5 },
            
            {"name": "Friday", "id": 6 },
            
            {"name": "Saturday", "id": 7 }
            ],
    
        "multiplesOfTen": [
            {"name": "Ten", "id": 10 },
            
            {"name": "Twenty", "id": 20 },
            
            {"name": "Thirty", "id": 30 },
            
            {"name": "Forty", "id": 40 },
            
            {"name": "Fifty", "id": 50 }
            ]
    };

    $scope.signals = {
        "todayIs": {
            "subscription_id": null,
            "name": "todayIs",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/signal/todayIs`
        },
    
        "randomWord": {
            "subscription_id": null,
            "name": "randomWord",
            "received": null,
            "received_time": null,
            "flash": false,
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/signal/randomWord`
        }
    };

    $scope.properties = {
        "favoriteNumber": {
            "subscription_id": null,
            "name": "favorite_number",
            "received": { 
                "number": null
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/favorite_number/value`,
            "update_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/favorite_number/update`,
            "response_topic": `client/${client_id}/Full/property/favorite_number/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "favoriteFoods": {
            "subscription_id": null,
            "name": "favorite_foods",
            "received": { 
                "drink": null,
            
                "slices_of_pizza": null,
            
                "breakfast": null
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/favorite_foods/value`,
            "update_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/favorite_foods/update`,
            "response_topic": `client/${client_id}/Full/property/favorite_foods/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "lunchMenu": {
            "subscription_id": null,
            "name": "lunch_menu",
            "received": { 
                "monday": { 
                    "drink": null,
                
                    "sandwich": null,
                
                    "crackers": null,
                
                    "day": null,
                
                    "order_number": null,
                
                    "time_of_lunch": null,
                
                    "duration_of_lunch": null
                 },
            
                "tuesday": { 
                    "drink": null,
                
                    "sandwich": null,
                
                    "crackers": null,
                
                    "day": null,
                
                    "order_number": null,
                
                    "time_of_lunch": null,
                
                    "duration_of_lunch": null
                 }
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/lunch_menu/value`,
            "flash": false,
            "property_version": -1
        },
    
        "familyName": {
            "subscription_id": null,
            "name": "family_name",
            "received": { 
                "family_name": null
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/family_name/value`,
            "update_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/family_name/update`,
            "response_topic": `client/${client_id}/Full/property/family_name/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "lastBreakfastTime": {
            "subscription_id": null,
            "name": "last_breakfast_time",
            "received": { 
                "timestamp": null
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/last_breakfast_time/value`,
            "update_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/last_breakfast_time/update`,
            "response_topic": `client/${client_id}/Full/property/last_breakfast_time/update/response`,
            "flash": false,
            "property_version": -1
        },
    
        "lastBirthdays": {
            "subscription_id": null,
            "name": "last_birthdays",
            "received": { 
                "mom": null,
            
                "dad": null,
            
                "sister": null,
            
                "brothers_age": null
             },
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/last_birthdays/value`,
            "update_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/property/last_birthdays/update`,
            "response_topic": `client/${client_id}/Full/property/last_birthdays/update/response`,
            "flash": false,
            "property_version": -1
        }
    };

    var interface_name = "Full";
    var client_id = clientId;
    // TODO: support all the topic params

    $scope.methods = {
        "addNumbers": {
            "name": "addNumbers",
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/method/addNumbers/request`,
            "response_topic": `client/${client_id}/Full/method/addNumbers/response`,
            "pending_correlation_id": null,
            "args": {
                "first": null,
            
                "second": null,
            
                "third": null
            },
            "received": null,
            "received_time": null
        },
        "doSomething": {
            "name": "doSomething",
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/method/doSomething/request`,
            "response_topic": `client/${client_id}/Full/method/doSomething/response`,
            "pending_correlation_id": null,
            "args": {
                "task_to_do": null
            },
            "received": null,
            "received_time": null
        },
        "whatTimeIsIt": {
            "name": "what_time_is_it",
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/method/what_time_is_it/request`,
            "response_topic": `client/${client_id}/Full/method/what_time_is_it/response`,
            "pending_correlation_id": null,
            "args": {},
            "received": null,
            "received_time": null
        },
        "holdTemperature": {
            "name": "hold_temperature",
            "mqtt_topic": `${topicParams.prefix}/Full/${topicParams.service_id}/method/hold_temperature/request`,
            "response_topic": `client/${client_id}/Full/method/hold_temperature/response`,
            "pending_correlation_id": null,
            "args": {
                "temperature_celsius": null
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

        const methodResponseSubscriptionTopic = `client/${client_id}/Full/method/+/response`;
        var methodResponseSubscriptionId = subscription_count++;
        const methodResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": methodResponseSubscriptionId
            }
        };
        client.subscribe(methodResponseSubscriptionTopic, methodResponseSubOpts);
        console.log("Subscribing to response topic " + methodResponseSubscriptionTopic + " with id " + methodResponseSubscriptionId);
        

        const propertyResponseSubscriptionTopic = `client/${client_id}/Full/property/+/update/response`;
        var propertyResponseSubscriptionId = subscription_count++;
        const propertyResponseSubOpts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": propertyResponseSubscriptionId
            }
        };
        client.subscribe(propertyResponseSubscriptionTopic, propertyResponseSubOpts);
        console.log("Subscribing to response topic " + propertyResponseSubscriptionTopic + " with id " + propertyResponseSubscriptionId);

        
        const today_is_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["todayIs"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/Full/${topicParams.service_id}/signal/todayIs`);
        client.subscribe(resolvedTopic, today_is_sub_opts);
        console.log("Subscribing to signal " + resolvedTopic + " with id ", subscription_count);
        subscription_count++;
        
        const random_word_sub_opts = {
            "qos": 1,
            "properties": {
                "subscriptionIdentifier": subscription_count
            }
        };

        $scope.signals["randomWord"].subscription_id = subscription_count;
        var resolvedTopic = resolveTopic(`${topicParams.prefix}/Full/${topicParams.service_id}/signal/randomWord`);
        client.subscribe(resolvedTopic, random_word_sub_opts);
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