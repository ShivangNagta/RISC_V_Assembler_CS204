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
    let { id, input_code } = req.body;
    let session;
    
    if (!id || !users.has(id)) {
        id = uuidv4();
        console.log("hello")
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
            session.output += data.toString();
            res.status(400).json({ error: session.output})
        });
    } else {
        session = users.get(id);
    }

    session.output = ""; 
    session.child.stdin.write("assemble\n");
    // const cod = {
    //     "input_code": "add x1, x2, x3\naddi x3, x4, 10\n"
    // }
    const newC = {
        "input_code": input_code
    }
    // console.log(JSON.stringify(newC))
    session.child.stdin.write(JSON.stringify(newC)+"\n");

    setTimeout(() => {
        const assembledJSON = JSON.parse(session.output)
        res.json({ machine_code: assembledJSON.machine_code, data_segment: assembledJSON.data_segment, id });
    }, 100);
});


app.post("/reset", (req,res) => {
    
})

app.post("/step", (req,res) => {

})

app.post("/run", (req,res) => {

})

app.listen(3000, () => console.log("Server running on port 3000"));
