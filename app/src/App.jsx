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
  const [pipelineEnabled, setPipelineEnabled] = useState(false);
  const [dataForwardPath, setDataForwardPath] = useState({});
  const [ra, setRA] = useState(0);
  const [rb, setRB] = useState(0);
  const [ry, setRY] = useState(0);
  const [rz, setRZ] = useState(0);
  const [rm, setRM] = useState(0);
  const [assembled, setAssembled] = useState(false)
  const [dataForwardingEnabled, setDataForwardingEnabled] = useState(false);
  const [branchPredictionEnabled, setBranchPredictionEnabled] = useState(false);
  const [pipelineInstructions, setPipelineInstructions] = useState({})

  const [totalInstructions, setTotalInstructions] = useState(0);
  const [totalDataTransferInstructions, setTotalDataTransferInstructions] = useState(0);
  const [totalControlInstructions, setTotalControlInstructions] = useState(0);
  const [totalBubbles, setTotalBubbles] = useState(0);
  const [totalControlHazardBubbles, setTotalControlHazardBubbles] = useState(0);
  const [totalDataHazardBubbles, setTotalDataHazardBubbles] = useState(0);
  const [totalDataHazards, setTotalDataHazards] = useState(0);
  const [totalControlHazards, setTotalControlHazards] = useState(0);
  const [totalBranchMissPredictions, setTotalBranchMissPredictions] = useState(0);

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
      setAssembled(true)
      let id = localStorage.getItem("id") ?? "";
      const response = await axios.post("https://risc-v-assembler-cs204.onrender.com/assemble", { id, code: editorContent });
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
    setAssembled(false);
    setPipelineEnabled(false)
    setDataForwardingEnabled(false)
    setBranchPredictionEnabled(false)
    setdataSegment({});
    setStack({});
    setRegisters({});
    setMachineCode([]);
    setComment("");
    setPipelineInstructions({});
    setClockCycles(0);
    setDataForwardPath({});
    setRA(0);
    setRB(0);
    setRY(0);
    setRZ(0);
    setRM(0);

    setTotalBranchMissPredictions(0);
    setTotalBubbles(0);
    setTotalControlHazardBubbles(0);
    setTotalControlHazards(0);
    setTotalControlInstructions(0);
    setTotalDataHazardBubbles(0);
    setTotalDataHazards(0);
    setTotalDataTransferInstructions(0);
    setTotalInstructions(0);
  };

  const handleStep = async () => {
    try {
      const id = localStorage.getItem("id") ?? "";
      const response = await axios.post("https://risc-v-assembler-cs204.onrender.com/step", { id });
      setdataSegment(response.data.data_segment);
      setStack(response.data.stack);
      setRegisters(response.data.registers);
      setClockCycles(response.data.clock_cycles);
      setComment(response.data.comment);
      setPipelineInstructions(response.data.pipeline_status)
      setDataForwardPath(response.data.data_forward_path)
      setRA(response.data.ra)
      setRB(response.data.rb)
      setRY(response.data.ry)
      setRZ(response.data.rz)
      setRM(response.data.rm)


    } catch (err) {
      setError(`${err.response?.data?.error ?? "Unknown Error"}`);
    }
  };

  const handleRun = async () => {
    try {
      const id = localStorage.getItem("id") ?? "";
      const response = await axios.post("https://risc-v-assembler-cs204.onrender.com/run", { id });
      setdataSegment(response.data.data_segment);
      setStack(response.data.stack);
      setRegisters(response.data.registers);
      setClockCycles(response.data.clock_cycles);
      setComment(response.data.comment);

      setTotalInstructions(response.data.totalInstructions);
      setTotalDataTransferInstructions(response.data.totalDataTransferInstructions);
      setTotalControlInstructions(response.data.totalControlInstructions);
      setTotalBubbles(response.data.totalBubbles);
      setTotalControlHazardBubbles(response.data.totalControlHazardBubbles);
      setTotalDataHazardBubbles(response.data.totalDataHazardBubbles);
      setTotalDataHazards(response.data.totalDataHazards);
      setTotalControlHazards(response.data.totalControlHazards);
      setTotalBranchMissPredictions(response.data.totalBranchMissPredictions);

    } catch (err) {
      setError(`${err.response?.data?.error ?? "Unknown Error"}`);
    }
  };

  const handleKeyDown = (e) => {
    if (e.key === "Tab") {
      e.preventDefault();
      
      // Get cursor position
      const start = e.target.selectionStart;
      const end = e.target.selectionEnd;
      
      const newText = editorContent.substring(0, start) + "    " + editorContent.substring(end);
      
      setEditorContent(newText);
      
      setTimeout(() => {
        e.target.selectionStart = e.target.selectionEnd = start + 4;
      }, 0);
    }
  };

  const togglePipeline = async () => {
    const newValue = !pipelineEnabled;
    const id = localStorage.getItem("id") ?? ""
    if (newValue === false && dataForwardingEnabled === true) toggleDataForwarding()
    console.log(newValue)
    setPipelineEnabled(newValue);
    const response = await axios.post(`https://risc-v-assembler-cs204.onrender.com/pipeline?pipelineEnabled=${newValue}`, { id });
  };

  const toggleDataForwarding = async () => {
    const id = localStorage.getItem("id") ?? "";
    const newValue = !dataForwardingEnabled;
    if (newValue === false && branchPredictionEnabled === true) toggleBranchPrediction()
    setDataForwardingEnabled(newValue);
    const response = await axios.post(`https://risc-v-assembler-cs204.onrender.com/dataForward/?dataForwardingEnabled=${newValue}`, { id });
  };

  const toggleBranchPrediction = async () => {
    const id = localStorage.getItem("id") ?? "";
    const newValue = !branchPredictionEnabled;
    setBranchPredictionEnabled(newValue);
    const response = await axios.post(`https://risc-v-assembler-cs204.onrender.com/branchPrediction/?branchPredictionEnabled=${newValue}`, { id });
  };

  const stageColor = {
    F: "bg-purple-700",
    D: "bg-yellow-700",
    E: "bg-red-700",
    M: "bg-green-700",
    W: "bg-pink-700",
  };

  const [activeTab, setActiveTab] = useState("simulator");
  const [rightTab, setRightTab] = useState("registers");

  return (
    <div className="w-full h-screen flex flex-col bg-black text-zinc-200 font-mono overflow-hidden">
      {/* Header/Navigation */}
      <div className="bg-zinc-950 border-b border-zinc-800 p-3 flex-shrink-0">
        <div className="flex flex-wrap justify-center max-w-4xl mx-auto">
          <div 
            onClick={() => setActiveTab("editor")} 
            className={`p-2 px-6 cursor-pointer rounded-t-lg text-center transition-colors duration-200 
              ${activeTab === "editor" ? "bg-zinc-800 text-zinc-100" : "bg-zinc-900 text-zinc-400 hover:bg-zinc-800 hover:text-zinc-300"}`}
          >
            Editor
          </div>
          <div 
            onClick={() => setActiveTab("simulator")} 
            className={`p-2 px-6 cursor-pointer rounded-t-lg text-center transition-colors duration-200
              ${activeTab === "simulator" ? "bg-zinc-800 text-zinc-100" : "bg-zinc-900 text-zinc-400 hover:bg-zinc-800 hover:text-zinc-300"}`}
          >
            Simulator
          </div>
        </div>
      </div>

      {/* Main Content */}
      <div className="flex-grow flex flex-col overflow-hidden">
        {activeTab === "editor" ? (
          <div className="h-full p-4 flex flex-col overflow-hidden">
            <div className="flex justify-between items-center mb-2 flex-shrink-0">
              <h2 className="text-zinc-300 text-lg">RISC-V Assembly Editor</h2>
              <button 
                onClick={assembleCode} 
                disabled={loading}
                className="px-4 py-2 bg-purple-700 hover:bg-purple-600 rounded text-white text-sm transition-colors flex items-center shadow-lg disabled:bg-zinc-700 disabled:cursor-not-allowed"
              >
                {loading ? "Assembling..." : "Assemble Code"}
              </button>
            </div>
            <textarea
              className="w-full flex-grow bg-zinc-900 p-4 text-green-400 border border-zinc-700 text-sm md:text-base resize-none rounded shadow-inner focus:outline-none focus:border-zinc-500 overflow-auto"
              value={editorContent}
              onChange={(e) => setEditorContent(e.target.value)}
              onKeyDown={handleKeyDown} // Add the key down handler here
              placeholder="Write your RISC-V code here..."
            />
            {error && (
              <div className="mt-2 p-2 bg-red-900 border border-red-700 text-red-100 rounded">
                {error}
              </div>
            )}
          </div>
        ) : (
          <div className="flex flex-col md:flex-row w-full h-full overflow-hidden">
            {/* Left panel - Simulator */}
            <div className="w-full md:w-2/3 border-r border-zinc-800 flex flex-col h-full overflow-hidden">
              {/* Control Panel */}
              <div className="bg-zinc-900 p-3 border-b border-zinc-800 flex flex-wrap justify-between items-center gap-2 flex-shrink-0">
                <div className="flex gap-2 flex-wrap">
                  <button 
                    onClick={handleStep} 
                    className="px-3 py-2 bg-zinc-800 hover:bg-zinc-700 rounded text-sm transition-colors shadow-md"
                  >
                    Step
                  </button>
                  <button 
                    onClick={handleRun} 
                    className="px-3 py-2 bg-zinc-800 hover:bg-zinc-700 rounded text-sm transition-colors shadow-md"
                  >
                    Run
                  </button>
                  <button 
                    onClick={handleReset} 
                    className="px-3 py-2 bg-zinc-800 hover:bg-zinc-700 rounded text-sm transition-colors shadow-md"
                  >
                    Reset
                  </button>
                </div>
                <div className="flex gap-2 flex-wrap">
                  <button
                    onClick={togglePipeline}
                    disabled={!assembled}
                    className={`px-3 py-2 rounded text-sm transition-colors shadow-md
                      ${assembled 
                        ? (pipelineEnabled ? "bg-green-700 hover:bg-green-600" : "bg-zinc-700 hover:bg-zinc-600") 
                        : "bg-zinc-900 text-zinc-600 cursor-not-allowed"
                      }`}
                  >
                    Pipeline {pipelineEnabled ? "On" : "Off"}
                  </button>
                  <button
                    onClick={toggleDataForwarding}
                    disabled={!pipelineEnabled}
                    className={`px-3 py-2 rounded text-sm transition-colors shadow-md
                      ${pipelineEnabled
                        ? (dataForwardingEnabled ? "bg-green-700 hover:bg-green-600" : "bg-zinc-700 hover:bg-zinc-600")
                        : "bg-zinc-900 text-zinc-600 cursor-not-allowed"
                      }`}
                  >
                    Forwarding {dataForwardingEnabled ? "On" : "Off"}
                  </button>
                  <button
                    onClick={toggleBranchPrediction}
                    disabled={!dataForwardingEnabled}
                    className={`px-3 py-2 rounded text-sm transition-colors shadow-md
                      ${dataForwardingEnabled
                        ? (branchPredictionEnabled ? "bg-green-700 hover:bg-green-600" : "bg-zinc-700 hover:bg-zinc-600")
                        : "bg-zinc-900 text-zinc-600 cursor-not-allowed"
                      }`}
                  >
                    Branch Prediction {branchPredictionEnabled ? "On" : "Off"}
                  </button>
                </div>
              </div>

              {/* Instruction Table */}
              <div className="overflow-auto flex-grow bg-zinc-950 relative">
                <div className="min-w-full h-full">
                  <table className="min-w-full text-center text-sm border-collapse">
                    <thead className="sticky top-0 bg-zinc-950 z-10">
                      <tr className="border-b border-zinc-800">
                        <th className="p-3 text-zinc-400 w-1/4">PC</th>
                        <th className="p-3 text-zinc-400 w-1/2">Machine Code</th>
                        <th className="p-3 text-zinc-400 w-1/4">Stage</th>
                      </tr>
                    </thead>
                    <tbody className="divide-y divide-zinc-800">
                      {machineCode.map((inst, index) => {
                        const currentStage = Object.entries(pipelineInstructions).find(
                          ([stage, pc]) => pc === inst.pc && pc !== "0x00002710"
                        )?.[0];

                        return (
                          <tr 
                            key={index} 
                            className={`${stageColor[currentStage] || "hover:bg-zinc-900"} border-b border-zinc-800 transition-colors`}
                          >
                            <td className="p-2 font-mono">{inst.pc}</td>
                            <td className="p-2 font-mono">{inst.machineCode}</td>
                            <td className="p-2 capitalize">{currentStage || '-'}</td>
                          </tr>
                        );
                      })}
                      {machineCode.length === 0 && (
                        <tr>
                          <td colSpan="3" className="p-6 text-zinc-500 text-center">
                            No instructions loaded. Assemble your code first.
                          </td>
                        </tr>
                      )}
                    </tbody>
                  </table>
                </div>
              </div>

              {/* Status Panels */}
              <div className="flex-shrink-0 p-2 bg-zinc-950">
                <div className="bg-zinc-950 text-lime-400 p-2 border border-zinc-800 rounded h-10 overflow-auto text-sm shadow-inner">
                  <strong className="text-zinc-400">Clock Cycles: </strong>{clockCycles}
                </div>
                <div className="mt-2 bg-zinc-950 text-lime-400 p-2 border border-zinc-800 rounded h-34 overflow-auto text-sm shadow-inner">
                  <strong className="text-zinc-400">Console:</strong>
                  <div className="whitespace-pre-wrap mt-1">{comment || "No output"}</div>
                </div>
                {error && (
                  <div className="mt-2 p-2 bg-red-900 border border-red-700 text-red-100 rounded">
                    {error}
                  </div>
                )}
              </div>
            </div>

            {/* Right panel - Registers/Memory */}
            <div className="w-full md:w-1/3 flex flex-col h-full bg-zinc-950 overflow-hidden">
              <div className="flex border-b border-zinc-800 flex-shrink-0">
                <div 
                  onClick={() => setRightTab("registers")} 
                  className={`p-3 cursor-pointer flex-1 text-center transition-colors
                    ${rightTab === "registers" ? "bg-zinc-800 text-zinc-100" : "bg-zinc-900 text-zinc-400 hover:bg-zinc-800"}`}
                >
                  Registers
                </div>
                <div 
                  onClick={() => setRightTab("memory")} 
                  className={`p-3 cursor-pointer flex-1 text-center transition-colors
                    ${rightTab === "memory" ? "bg-zinc-800 text-zinc-100" : "bg-zinc-900 text-zinc-400 hover:bg-zinc-800"}`}
                >
                  Memory
                </div>
                <div 
                  onClick={() => setRightTab("stats")} 
                  className={`p-3 cursor-pointer flex-1 text-center transition-colors
                    ${rightTab === "stats" ? "bg-zinc-800 text-zinc-100" : "bg-zinc-900 text-zinc-400 hover:bg-zinc-800"}`}
                >
                  Stats
                </div>
              </div>

              {/* Content based on selected tab - Scrollable */}
              <div className="overflow-auto flex-grow m-2">
                <div className="border border-zinc-800 p-3 text-sm bg-zinc-950 shadow-inner rounded h-full">
                {rightTab !== "stats" ? (
  rightTab === "registers" ? (
    <>
      {/* Registers Tab */}
      <h1 className="mb-3 text-xl font-bold text-zinc-300 border-b border-zinc-800 pb-2 sticky top-0 bg-zinc-950 z-10">
        Registers
      </h1>
      <div className="space-y-1">
        {Object.entries(registers).length > 0 ? (
          Object.entries(registers).map(([name, value], i) => (
            <div
              key={i}
              className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded"
            >
              <span className="font-mono text-zinc-300">{name}</span>
              <span className="font-mono text-lime-400">{value}</span>
            </div>
          ))
        ) : (
          <div className="text-zinc-500 text-center py-4">
            No register data available
          </div>
        )}
      </div>
    </>
  ) : (
    <>
      {/* Stack and Data Segment Tab */}
      <div className="mb-6">
        <h2 className="text-zinc-300 mb-2 font-bold border-b border-zinc-800 pb-1 sticky top-0 bg-zinc-950 z-10">
          Stack Segment
        </h2>
        {Object.entries(stack).length > 0 ? (
          Object.entries(stack).map(([addr, value], i) => (
            <div
              key={i}
              className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded"
            >
              <span className="font-mono text-zinc-300">{addr}</span>
              <span className="font-mono text-lime-400">{value}</span>
            </div>
          ))
        ) : (
          <div className="text-zinc-500 text-center py-2">
            No stack data available
          </div>
        )}
      </div>

      <div>
        <h2 className="text-zinc-300 mb-2 font-bold border-b border-zinc-800 pb-1 sticky top-0 bg-zinc-950 z-10">
          Data Segment
        </h2>
        {Object.entries(dataSegment).length > 0 ? (
          Object.entries(dataSegment).map(([addr, value], i) => (
            <div
              key={i}
              className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded"
            >
              <span className="font-mono text-zinc-300">{addr}</span>
              <span className="font-mono text-lime-400">{value}</span>
            </div>
          ))
        ) : (
          <div className="text-zinc-500 text-center py-2">
            No data segment available
          </div>
        )}
      </div>
    </>
  )
) : (
  <>
    {/* Stats Tab */}
    <div className="text-zinc-300 text-center py-4">
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Total Cycles</span>
            <span className="font-mono text-lime-400">{clockCycles}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Total Instructions Executed</span>
            <span className="font-mono text-lime-400">{totalInstructions}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">CPI</span>
            <span className="font-mono text-lime-400">{(clockCycles / totalInstructions).toFixed(2)}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of Data-transfer instructions</span>
            <span className="font-mono text-lime-400">{totalDataTransferInstructions}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of Control instructions</span>
            <span className="font-mono text-lime-400">{totalControlInstructions}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of stalls/bubbles</span>
            <span className="font-mono text-lime-400">{totalBubbles}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of data hazards</span>
            <span className="font-mono text-lime-400">{totalDataHazards}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of control hazards</span>
            <span className="font-mono text-lime-400">{totalControlHazards}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of branch mispredictions</span>
            <span className="font-mono text-lime-400">{totalBranchMissPredictions}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of stalls due to data hazards</span>
            <span className="font-mono text-lime-400">{totalDataHazardBubbles}</span>
      </div>
      <div className="flex justify-between py-1 hover:bg-zinc-900 px-2 rounded">
            <span className="font-mono text-zinc-300">Number of stalls due to control hazards</span>
            <span className="font-mono text-lime-400">{totalControlHazardBubbles}</span>
      </div>
    </div>
  </>
)}

                </div>
              </div>

              {/* Pipeline Registers and Data Forwarding - Fixed height sections */}
              <div className="flex-shrink-0 overflow-hidden">
                {/* Pipeline Registers Panel */}
                <div className="bg-zinc-950 border border-zinc-800 rounded shadow-inner m-2 overflow-auto max-h-70">
                  <h1 className="p-2 text-lg font-bold text-zinc-300 border-b border-zinc-800 sticky top-0 bg-zinc-950 z-10">Pipeline Registers</h1>
                  <div className="p-2">
                    <div className="grid grid-cols-2 gap-2">
                      <div className="bg-zinc-900 p-2 rounded border border-zinc-800">
                        <div className="text-zinc-400 text-xs mb-1">RA</div>
                        <div className="font-mono text-lime-400">{ra}</div>
                      </div>
                      <div className="bg-zinc-900 p-2 rounded border border-zinc-800">
                        <div className="text-zinc-400 text-xs mb-1">RB</div>
                        <div className="font-mono text-lime-400">{rb}</div>
                      </div>
                      <div className="bg-zinc-900 p-2 rounded border border-zinc-800">
                        <div className="text-zinc-400 text-xs mb-1">RY</div>
                        <div className="font-mono text-lime-400">{ry}</div>
                      </div>
                      <div className="bg-zinc-900 p-2 rounded border border-zinc-800">
                        <div className="text-zinc-400 text-xs mb-1">RZ</div>
                        <div className="font-mono text-lime-400">{rz}</div>
                      </div>
                      <div className="bg-zinc-900 p-2 rounded border border-zinc-800 col-span-2">
                        <div className="text-zinc-400 text-xs mb-1">RM</div>
                        <div className="font-mono text-lime-400">{rm}</div>
                      </div>
                    </div>
                  </div>
                </div>

                {/* Data Forwarding Panel */}
                <div className="mb-2 bg-zinc-950 border border-zinc-800 rounded shadow-inner m-2 overflow-auto max-h-34">
                  <h1 className="p-2 text-lg font-bold text-zinc-300 border-b border-zinc-800 sticky top-0 bg-zinc-950 z-10">Data Forwarding</h1>
                  <div className="p-4">
                    {dataForwardPath.fromBuffer === undefined || dataForwardPath.fromBuffer === '' ? (
                      <div className="text-zinc-500">No Data Forwarding</div>
                    ) : (
                      <div className="text-lime-400">
                        Data Forwarding from <span className="font-bold">{dataForwardPath.fromBuffer}</span> to <span className="font-bold">{dataForwardPath.toBuffer}</span>
                      </div>
                    )}
                  </div>
                </div>
              </div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}