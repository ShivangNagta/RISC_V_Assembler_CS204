import { useState } from "react";
import axios from "axios";

export default function App() {
  const [machineCode, setMachineCode] = useState([]);
  const [dataSegment, setdataSegment] = useState({});
  const [stack, setStack] = useState({});
  const [registers, setRegisters] = useState({});
  const [clockCycles, setClockCycles] = useState(0);
  const [comment, setComment] = useState("");
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
    jal ra, loop      # Repeat

done:
    jal ra, done      # Infinite loop (halt simulation)`);

  const assembleCode = async () => {
    setLoading(true);
    setError("");
    try {
      let id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/assemble", { id, code: editorContent });
      setMachineCode(response.data.machine_code);
      setdataSegment(response.data.data_segment);
      localStorage.setItem("id", response.data.id);
    } catch (err) {
      setError(`${err.response?.data?.error ?? "Unknown Error"}`);
    }
    setLoading(false);
  };

  const handleReset = () => {
    localStorage.setItem("id", "");
    setdataSegment({});
    setStack({});
    setRegisters({});
    setMachineCode([]);
    setComment("");
    setClockCycles(0);
  };

  const handleStep = async () => {
    try {
      const id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/step", { id });
      setdataSegment(response.data.data_segment);
      setStack(response.data.stack);
      setRegisters(response.data.registers);
      setClockCycles(response.data.clock_cycles);
      setComment(response.data.comment);
    } catch (err) {
      setError(`${err.response?.data?.error ?? "Unknown Error"}`);
    }
  };

  const handleRun = async () => {
    try {
      const id = localStorage.getItem("id") ?? "";
      const response = await axios.post("http://localhost:3000/run", { id });
      setdataSegment(response.data.data_segment);
      setStack(response.data.stack);
      setRegisters(response.data.registers);
      setClockCycles(response.data.clock_cycles);
      setComment(response.data.comment);
    } catch (err) {
      setError(`${err.response?.data?.error ?? "Unknown Error"}`);
    }
  };

  const [activeTab, setActiveTab] = useState("simulator");
  const [rightTab, setRightTab] = useState("registers");

  return (
    <div className="p-4 w-full h-screen flex flex-col bg-black text-gray-200 font-mono overflow-hidden">
      {/* Tabs */}
      <div className="flex flex-wrap justify-center md:w-2/3 border-b border-gray-700 mb-4 mx-auto">
        <div
          onClick={() => setActiveTab("editor")}
          className={`p-2 cursor-pointer flex-1 text-center ${activeTab === "editor" ? "bg-gray-800" : "bg-gray-900"
            }`}
        >
          Editor
        </div>
        <div
          onClick={() => setActiveTab("simulator")}
          className={`p-2 cursor-pointer flex-1 text-center ${activeTab === "simulator" ? "bg-gray-800" : "bg-gray-900"
            }`}
        >
          Simulator
        </div>
      </div>

      {/* Editor Area */}
      {activeTab === "editor" ? (
        <textarea
          className="w-full h-full bg-gray-900 p-3 text-green-400 border border-gray-700 text-sm md:text-base resize-none"
          value={editorContent}
          onChange={(e) => setEditorContent(e.target.value)}
          placeholder="Write your RISC-V code here..."
        />
      ) : (
        // Simulator Layout
        <div className="flex flex-col md:flex-row w-full h-full overflow-hidden">
          {/* Left Simulator Panel */}
          <div className="w-full md:w-2/3 border-r border-gray-700 p-2 flex flex-col h-full">
            {/* Buttons */}
            <div className="flex flex-wrap justify-center gap-2 bg-gray-900 p-2 sticky top-0 z-10">
              <button onClick={handleStep} className="px-3 py-1 bg-gray-800 hover:bg-gray-700 rounded text-sm">
                Step
              </button>
              <button onClick={handleRun} className="px-3 py-1 bg-gray-800 hover:bg-gray-700 rounded text-sm">
                Run
              </button>
              <button onClick={handleReset} className="px-3 py-1 bg-gray-800 hover:bg-gray-700 rounded text-sm">
                Reset
              </button>
              <button
                onClick={assembleCode}
                className="px-3 py-1 bg-indigo-700 hover:bg-indigo-600 rounded text-white text-sm"
              >
                Assemble
              </button>
            </div>

            {/* Machine Code Table */}
            <div className="overflow-auto flex-grow border border-gray-700">
              <table className="w-full text-center text-sm border-collapse">
                <thead className="sticky top-0 bg-gray-800">
                  <tr className="border-b border-gray-700">
                    <th className="p-2">PC</th>
                    <th className="p-2">Machine Code</th>
                  </tr>
                </thead>
                <tbody>
                  {machineCode.map((inst, index) => (
                    <tr key={index} className="border-b border-gray-800">
                      <td className="p-2">{inst.pc}</td>
                      <td className="p-2">{inst.machineCode}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>

            {/* Console Output */}
            <div className="mt-2 bg-black text-lime-400 p-2 border-t border-l border-r border-gray-700 rounded-b h-10 overflow-auto text-sm">
              <strong className="text-gray-400">Clock Cycles : {clockCycles}</strong>
            </div>
            <div className="mt-2 bg-black text-lime-400 p-2 border-t border-l border-r border-gray-700 rounded-b h-24 overflow-auto text-sm">
              <strong className="text-gray-400">Console:</strong>
              <div className="whitespace-pre-wrap">{comment}</div>
            </div>
          </div>

          {/* Right Memory/Register Panel */}
          <div className="w-full md:w-1/3 p-2 flex flex-col h-full border-l border-gray-700">
            {/* Tabs */}
            <div className="flex border-b border-gray-700 justify-center mb-2">
              <div
                onClick={() => setRightTab("registers")}
                className={`p-2 cursor-pointer flex-1 text-center ${rightTab === "registers" ? "bg-gray-800" : "bg-gray-900"
                  }`}
              >
                Registers
              </div>
              <div
                onClick={() => setRightTab("memory")}
                className={`p-2 cursor-pointer flex-1 text-center ${rightTab === "memory" ? "bg-gray-800" : "bg-gray-900"
                  }`}
              >
                Memory
              </div>
            </div>

            {/* Right Tab Content */}
            <div className="overflow-auto max-h-screen border border-gray-700 p-2 text-sm">
              {rightTab === "registers" ? (
                Object.entries(registers).map(([name, value], i) => (
                  <div key={i} className="flex justify-between border-b border-gray-700 py-1">
                    <span>{name}</span>
                    <span>{value}</span>
                  </div>
                ))
              ) : (
                <>
                  <div className="mb-3">
                    <h2 className="text-gray-400">Stack</h2>
                    {Object.entries(stack).map(([addr, value], i) => (
                      <div key={i} className="flex justify-between border-b border-gray-700 py-1">
                        <span>{addr}</span>
                        <span>{value}</span>
                      </div>
                    ))}
                  </div>
                  <div>
                    <h2 className="text-gray-400">Data Segment</h2>
                    {Object.entries(dataSegment).map(([addr, value], i) => (
                      <div key={i} className="flex justify-between border-b border-gray-700 py-1">
                        <span>{addr}</span>
                        <span>{value}</span>
                      </div>
                    ))}
                  </div>
                </>
              )}
            </div>
          </div>
        </div>
      )}
    </div>

  );
}
