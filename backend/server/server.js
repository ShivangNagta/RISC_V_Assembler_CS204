const express = require("express");
const cors = require("cors");
const fs = require("fs");
const { exec } = require("child_process");

const app = express();
app.use(express.json());
app.use(cors());

app.post("/assemble", (req, res) => {
    const { code } = req.body;
    const inputFile = "input.asm";
    const outputFile = "output.mc";

    // Save input assembly to a file
    fs.writeFileSync(inputFile, code);

    // Call the assembler binary
    exec(`../main ${inputFile} ${outputFile}`, (error, stdout, stderr) => {
        if (error) {
            console.error("Assembler Error:", stderr);
            return res.status(500).json({ error: stderr });
        }
        const output = fs.readFileSync(outputFile, "utf8");
        res.json({ machineCode: output });
    });
});

app.listen(3000, () => console.log("Server running on port 5000"));
