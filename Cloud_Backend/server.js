const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(cors());

let lastSeen = { zones: "", timestamp: "" };

app.post('/updateLocation', (req, res) => {
    const { zones, timestamp } = req.body;
    if(zones && timestamp){
        lastSeen = { zones, timestamp };
        console.log("Updated:", lastSeen);
        res.status(200).send({ status: "success", data: lastSeen });
    } else {
        res.status(400).send({ status: "error", message: "Invalid payload" });
    }
});

app.get('/getLocation', (req, res) => {
    res.status(200).send(lastSeen);
});

app.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
