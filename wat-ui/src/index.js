const express = require("express");
const multer = require("multer");
const { spawn } = require("child_process");
const fs = require("fs");

const app = express();
const storage = multer.diskStorage({
    destination(_req, _file, cb) {
        fs.mkdir("uploads-temp/", { recursive: true }, (err) => {
            if (err) console.error(`error creating uploads-temp directory: `, err);
        });
        cb(null, "uploads-temp/");
    },
    filename(_req, file = {}, cb) {
        const dn = Date.now();
        fs.mkdir(`uploads-temp/${dn}`, { recursive: true }, (err) => {
            if (err) console.error(`error creating uploads-temp/${dn} directory: `, err);
        });
        cb(null, `${dn}/${file.originalname}`);
    },
});
const upload = multer({ storage });

const port = 3000;

app.use(express.static("public"));

app.get("/", (_req, res) => {
    res.send("index.html");
});

app.post("/process", upload.single("file"), (req, res) => {
    const mode = parseInt(req.body.mode);
    const spf = parseInt(req.body.spf);
    const file = req.file;

    const watProcess = spawn("./wat-bin/output", [mode, spf, file.path]);
    let outputFilePath = null;

    watProcess.stdout.on("data", (data) => {
        outputFilePath = data.toString();
    });

    watProcess.on("close", (code) => {
        if (code != 0 || outputFilePath == null) {
            console.error("wat exited with code: ", code);
            return res.status(500).json({msg: "Internal Server Error"});
        }
        let filename = outputFilePath.trim();
        fs.readFile(filename, (err, _data) => {
            if (err) {
                console.error("error reading file: ", err);
                return res.status(500).json({msg: "Internal Server Error"});
            }
            
            return res.download(filename, (err) => {
                if (err) {
                    console.error("error downloading file: ", err);
                    return res.status(500).json({msg: "Internal Server Error"});
                }

                fs.rm(filename.match(/(.*)[\/\\]/)[1] || [], { recursive: true }, (err) => {
                    if (err) console.error(`error deleting directory: `, err);
                });
            });
        });
    });
});

app.listen(port, () => {
    console.log(`Listening at http://localhost:${port}`);
    if (fs.existsSync('./wat-bin/output') || fs.existsSync('./wat-bin/output.*')) {
        console.error("wat binary not found");
        process.exit(1);
    }
});