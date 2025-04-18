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
.word 6

.text

_start:
        # Load n from memory (assume at address 0x10010000)
        lui     x16, 0x10000       # x16 = upper part of 0x10010000
        lw      x10, 0(x16)        # x10 = n

        # Initialize fib(0) = 0, fib(1) = 1
        addi    x11, x0, 1         # fib(i-1) = 1
        addi    x12, x0, 0         # fib(i-2) = 0
        addi    x13, x0, 2         # i = 2

        # Memory pointer for storing results (0x10010004)
        addi    x15, x16, 4
        sw      x12, 0(x15)        # store fib(0)
        sw      x11, 4(x15)        # store fib(1)

        # Preload shift constant for sll (left shift by 2 = multiply by 4)
        addi    x20, x0, 2         # x20 = shift amount

        # if n <= 2, skip loop
        addi    x17, x0, 2
        bge     x17, x10, done     # if n <= 2, jump to done

loop:
        add     x14, x11, x12      # fib(i) = fib(i-1) + fib(i-2)

        # Calculate offset = i * 4 using sll
        sll     x18, x13, x20      # x18 = i << 2
        add     x19, x15, x18      # x19 = memory address to store fib(i)
        sw      x14, 0(x19)        # store fib(i)

        # Update fib(i-2) = fib(i-1), fib(i-1) = fib(i)
        addi    x12, x11, 0        # x12 = previous x11
        addi    x11, x14, 0        # x11 = new fib(i)

        addi    x13, x13, 1        # i++

        blt     x13, x10, loop     # if i < n, continue

done:
        exit x0     # infinite loop to end program

    `);

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

{error && (
  <div className="fixed inset-0 flex items-center justify-center z-50 bg-black/50">
    <div className="bg-gray-900/90 border border-gray-700 text-red-400 rounded-lg shadow-xl p-6 max-w-md w-full">
      <h2 className="text-lg font-semibold mb-2 text-red-500">Error</h2>
      <p className="mb-4 whitespace-pre-wrap text-sm">{error}</p>
      <button
        onClick={() => setError("")}
        className="bg-gray-800 hover:bg-gray-700 text-gray-200 px-4 py-1 rounded text-sm"
      >
        OK
      </button>
    </div>
  </div>
)}




    </div>

  );
}
