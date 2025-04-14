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
        console.log(session.output)
        if (error) {
            res.status(400).json({ error: session.output });
        }else{
            try {
                const assembledJSON = JSON.parse(session.output);
                console.log(assembledJSON)
                res.json({
                    machine_code: assembledJSON.machine_code,
                    data_segment: assembledJSON.data_segment,
                    id,
                });
            } catch (err) {
                res.status(500).json({ error: "Invalid output from assembler" });
            }
        }
    }, 1000);
});

app.post("/step", (req,res) => {
    let { id } = req.body
    if (!id || !users.has(id)) return res.status(400).json({ error: "Cannot step, you need to assemble first"})

    const session = users.get(id)
    session.output = ""
    session.child.stdin.write("step\n");

    // console.log(session.output)
    // const outputJSON = JSON.parse(session.output);
    // console.log(outputJSON)

    setTimeout(() => {
        console.log(session.output)
        try {
            const outputJSON = JSON.parse(session.output);
            console.log(outputJSON)
            res.json({
                data_segment: outputJSON.data_segment,
                instruction_memory: outputJSON.instruction_memory,
                stack: outputJSON.stack,
                registers: outputJSON.registers,
                clock_cycles: outputJSON.clock_cycles,
                comment: outputJSON.comment,
            });
        } catch (err) {
            res.status(500).json({ error: session.output });
        }
    }, 500);
    
})

app.post("/run", (req,res) => {
    let { id } = req.body
    if (!id || !users.has(id)) return res.status(400).json({ error: "Cannot run, you need to assemble first"})

    const session = users.get(id)
    session.output = ""
    session.child.stdin.write("run\n");

    setTimeout(() => {
        console.log(session.output)
        try {
            const outputJSON = JSON.parse(session.output);
            console.log(outputJSON)
            res.json({
                data_segment: outputJSON.data_segment,
                instruction_memory: outputJSON.instruction_memory,
                stack: outputJSON.stack,
                registers: outputJSON.registers,
                clock_cycles: outputJSON.clock_cycles,
                comment: "Finished",
            });
        } catch (err) {
            res.status(500).json({ error: session.output });
        }
    }, 500);
    
})

app.listen(3000, () => console.log("Server running on port 3000"));
