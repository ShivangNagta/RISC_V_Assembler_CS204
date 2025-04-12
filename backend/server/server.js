import express from 'express'
import cors from 'cors'
import path from 'path'
import { spawn } from 'child_process'
import { v4 as uuidv4 } from 'uuid';
import { fileURLToPath } from 'url';

const app = express();
app.use(express.json());
app.use(cors());

let users = new Map()

app.post("/assemble", (req, res) => {
    let { id, code } = req.body
    let session
    let error = false
    console.log("hello")
    if (!id || !users.has(id)) {
        id = uuidv4();
        const __filename = fileURLToPath(import.meta.url);
        const __dirname = path.dirname(__filename);
        const assemblerPath = path.resolve(__dirname, "../main");

        const child = spawn(assemblerPath);
        session = { child, output: "" };
        users.set(id, session);

        child.stdout.on("data", (data) => {
            session.output += data.toString();
        });

        child.stderr.on("data", (data) => {
            error = true
            session.output += data.toString();
        });

    } else {
        session = users.get(id);
    }

    session.output = ""; 
    session.child.stdin.write("assemble\n");
    // const cod = {
    //     "input_code": "add x1, x2, x3\naddi x3, x4, 10\n"
    // }
    const input_code = {
        "input_code": code
    }
    // console.log(JSON.stringify(input_code))
    session.child.stdin.write(JSON.stringify(input_code)+"\n");

    console.log([...users.keys()])


    setTimeout(() => {
        if (error) {
            res.status(400).json({ error: session.output });
        }else{
            try {
                const assembledJSON = JSON.parse(session.output);
                console.log(assembledJSON.data_segment)
                res.json({
                    machine_code: assembledJSON.machine_code,
                    data_segment: assembledJSON.data_segment,
                    id,
                });
            } catch (err) {
                res.status(500).json({ error: "Invalid output from assembler", raw: session.output });
            }
        }
    }, 100);
});

app.post("/reset", (req,res) => {
    
})

app.post("/step", (req,res) => {

})

app.post("/run", (req,res) => {

})

app.listen(3000, () => console.log("Server running on port 3000"));
