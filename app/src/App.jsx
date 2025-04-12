import { useState } from "react";
import axios from "axios";

export default function App() {
  // const [assemblyCode, setAssemblyCode] = useState("");
  const [machineCode, setMachineCode] = useState([]);
  const [dataSegment, setdataSegment] = useState({})
  const [stack, setStack] = useState({})
  const [registers, setRegisters] = useState({})
  const [clockCycles, setClockCycles] = useState(0)
  const [comment, setComment] = useState({})
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState("");
  const [editorContent, setEditorContent] = useState(`#Default

.data
 hello: .word 10, 20

.text

_start:
    # Initialize Fibonacci sequence
    addi x0, x5, 0      # F(0) = 0
    addi x0, x6, 1      # F(1) = 1
    addi x0, x7, 10     # Set n (change this value for more terms)
    
    add x10, x5, x0  # Copy F(0) to x10
    add x11, x6, x0  # Copy F(1) to x11

loop:
    beq x7, x0, done  # If n == 0, exit
    add x12, x10, x11 # F(n) = F(n-1) + F(n-2)
    
    # Shift values for the next iteration
    add x10, x11, x0  # F(n-1) becomes F(n-2)
    add x11, x12, x0  # F(n) becomes F(n-1)
    
    addi x7, x7, -1   # Decrement counter
    jal ra, loop            # Repeat

done:
    jal ra, done            # Infinite loop (halt simulation)`);

  const assembleCode = async () => {
    setLoading(true);
    setError("");

    try {
      let id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/assemble", { id, code: editorContent });
      setMachineCode(response.data.machine_code);
      setdataSegment(response.data.data_segment)
      console.log(response.data.data_segment)
      id = response.data.id
      localStorage.setItem("id", id)
    } catch (err) {
      console.log(err.response.data.error)
      setError(`${err.response.data.error}`);
    }
    setLoading(false);
  };

  const handleReset = () => {
    localStorage.setItem("id", "")
    setdataSegment({})
    setMachineCode([]);
  }

  const handleStep = async () => {
    try {
      let id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/step", { id });

      console.log(response.data)

      setdataSegment(response.data.data_segment)
      setStack(response.data.stack)
      setRegisters(response.data.registers)
      setClockCycles(response.data.clock_cycles)
      setComment(response.data.comment)

      // console.log(registers)
      // console.log(clockCycles)

    } catch (err) {
      console.log(err.response.data.error)
      setError(`${err.response.data.error}`);
    }
  }

  const handleRun = async () => {
    try {
      let id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/run", { id });

      console.log(response.data)

      setdataSegment(response.data.data_segment)
      setStack(response.data.stack)
      setRegisters(response.data.registers)
      setClockCycles(response.data.clock_cycles)
      setComment(response.data.comment)

      // console.log(registers)
      // console.log(clockCycles)

    } catch (err) {
      console.log(err.response.data.error)
      setError(`${err.response.data.error}`);
    }
  }

  const [activeTab, setActiveTab] = useState("simulator");
  const [rightTab, setRightTab] = useState("registers");


  // const registers = Array.from({ length: 32 }, (_, i) => ({ name: `x${i}`, value: "0x00000000" }));


  return (

    <div className="p-4 w-full h-screen flex flex-col bg-gray-900 text-gray-200 font-mono overflow-hidden">
      {error && (
        <div className="fixed top-1/4 left-1/2 transform -translate-x-1/2 -translate-y-1/4 bg-red-100 text-red-800 border border-red-400 p-6 rounded-xl shadow-xl z-50 min-w-[300px] text-center">
          <h2 className="text-lg font-bold mb-2">Error</h2>
          <p className="mb-4">{error}</p>
          <button
            onClick={() => setError("")}
            className="bg-red-500 hover:bg-red-600 text-white font-semibold py-1.5 px-4 rounded"
          >
            OK
          </button>
        </div>
      )}



      <style>{`
        ::-webkit-scrollbar {
          width: 0px;
          background: transparent;
        }
        ::-webkit-scrollbar-thumb {
          background: transparent;
        }
      `}</style>
      <div className="flex w-2/3 border-b border-gray-700 mb-4 justify-center">
        <div
          onClick={() => setActiveTab("editor")}
          className={`p-2 cursor-pointer flex-1 text-center ${activeTab === "editor" ? "bg-gray-700" : "bg-gray-800"}`}
        >
          Editor
        </div>
        <div
          onClick={() => setActiveTab("simulator")}
          className={`p-2 cursor-pointer flex-1 text-center ${activeTab === "simulator" ? "bg-gray-700" : "bg-gray-800"}`}
        >
          Simulator
        </div>
      </div>
      {activeTab === "editor" ? (
        <textarea
          className="w-full h-full bg-gray-800 p-2 text-gray-200 border-2 border-gray-700"
          value={editorContent}
          onChange={(e) => setEditorContent(e.target.value)}
          placeholder="Write your RISC-V code here..."
        />
      ) : (
        <div className="flex w-full h-full overflow-hidden">
          <div className="w-2/3 border-r border-gray-700 p-2 flex flex-col h-full">
            <div className="flex justify-center space-x-2 mb-2 bg-gray-800 p-2 sticky top-0 z-10">
              <button onClick={handleStep} className="p-2 flex-1 bg-gray-700">Step</button>
              <button onClick={handleRun} className="p-2 flex-1 bg-gray-700">Run</button>
              <button onClick={handleReset} className="p-2 flex-1 bg-gray-700">Reset</button>
              <button onClick={assembleCode} className="p-2 flex-2 bg-gray-700">Assemble</button>
            </div>
            <div className="overflow-auto flex-grow">
              <table className="w-full text-center border-collapse">
                <thead>
                  <tr className="border-b border-gray-700">
                    <th className="p-2">PC</th>
                    <th className="p-2">Machine Code</th>
                  </tr>
                </thead>
                <tbody>
                  {machineCode.map((inst, index) => (
                    <tr key={index} className="border-b border-gray-700">
                      <td className="p-2">{inst.pc}</td>
                      <td className="p-2">{inst.machineCode}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
          <div className="w-1/3 p-2 flex flex-col h-full border-l border-gray-700">
            <div className="flex border-b border-gray-700 justify-center mb-2">
              <div
                onClick={() => setRightTab("registers")}
                className={`p-2 cursor-pointer flex-1 text-center ${rightTab === "registers" ? "bg-gray-700" : "bg-gray-800"}`}
              >
                Registers
              </div>
              <div
                onClick={() => setRightTab("memory")}
                className={`p-2 cursor-pointer flex-1 text-center ${rightTab === "memory" ? "bg-gray-700" : "bg-gray-800"}`}
              >
                Memory
              </div>
            </div>
            {rightTab === "registers" ? (
              <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2">
                {Object.entries(registers).map((reg, i) => (
                  <div key={i} className="flex justify-between p-2 border-b border-gray-700 text-lg">
                    <span className="text-center flex-2">{reg.name}</span>
                    <span className="text-center flex-3">{reg.value}</span>
                  </div>
                ))}
              </div>
            ) : (
              <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2">
                <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2 mb-3">
                  <h2>Stack</h2>
                </div>

                <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2 mb-3">
                  <h2>Data Segment</h2>
                  {Object.entries(dataSegment).map(([addr, value], i) => (
                    <div key={i} className="flex justify-between p-2 border-b border-gray-700 ">
                      <span>{addr}</span>
                      <span>{value}</span>
                    </div>
                  ))}
                </div>

                <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2">
                  <h2>Text Segment</h2>
                  {machineCode.map((inst, index) => (
                    <div key={index} className="flex justify-between p-2 border-b border-gray-700 ">
                      <span>{inst.pc}</span>
                      <span>{inst.machineCode}</span>
                    </div>
                    ))}
                </div>

              </div>
            )}
          </div>
        </div>
      )}
    </div>
  );
}