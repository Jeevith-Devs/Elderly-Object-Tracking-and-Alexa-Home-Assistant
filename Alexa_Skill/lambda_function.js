const https = require('https');

exports.handler = async (event) => {
    if(event.request.type === "IntentRequest" && event.request.intent.name === "GetObjectLocationIntent"){
        const location = await getLocationFromCloud();
        const speechText = location.zones ? 
            `Your objects were last seen in: ${location.zones}.` : 
            "I could not find any objects at the moment.";

        return {
            version: "1.0",
            response: {
                outputSpeech: {
                    type: "PlainText",
                    text: speechText
                },
                shouldEndSession: true
            }
        };
    }
};

function getLocationFromCloud(){
    return new Promise((resolve, reject) => {
        https.get('https://your-backend.com/getLocation', (resp) => {
            let data = '';
            resp.on('data', (chunk) => { data += chunk; });
            resp.on('end', () => {
                resolve(JSON.parse(data));
            });
        }).on("error", (err) => {
            console.log("Error: " + err.message);
            resolve({});
        });
    });
}
