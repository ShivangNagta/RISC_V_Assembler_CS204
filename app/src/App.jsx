import { useState } from "react";
import axios from "axios";

export default function App() {
    const [assemblyCode, setAssemblyCode] = useState("");
    const [machineCode, setMachineCode] = useState("");
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState("");

    const assembleCode = async () => {
        setLoading(true);
        setError("");
        try {
            const response = await axios.post("http://localhost:3000/assemble", { code: assemblyCode });
            setMachineCode(formatMachineCode(response.data.machineCode));
        } catch (err) {
            console.log(err.response.data.error)
            setError(`${err.response.data.error}`);
        }
        setLoading(false);
    };

    // Format machine code properly with aligned addresses
    const formatMachineCode = (code) => {
        if (!code) return "";
        return code
            .split("\n")
            .map(line => {
                const parts = line.trim().split(/\s+/);
                if (parts.length === 2) {
                    return `<span class="text-green-400">${parts[0]}</span> <span class="text-yellow-300">${parts[1]}</span>`;
                }
                return `<span class="text-yellow-300">${line}</span>`;
            })
            .join("<br>");
    };

    const [activeTab, setActiveTab] = useState("simulator");
    const [rightTab, setRightTab] = useState("registers");
    const [editorContent, setEditorContent] = useState("");
  
    const registers = Array.from({ length: 32 }, (_, i) => ({ name: `x${i}`, value: "0x00000000" }));
    const memory = { "0x1000": "0xABCD", "0x1004": "0x1234" };
  
    const instructions = [
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x8", machineCode: "0x004180B3", originalCode: "add x1, x3, x4" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
      { pc: "0x0", machineCode: "0x00100093", originalCode: "addi x1, x0, 1" },
      { pc: "0x4", machineCode: "0x00309113", originalCode: "slli x2, x1, 3" },
    ];


    return (
        <div className="p-4 w-full h-screen flex flex-col bg-gray-900 text-gray-200 font-mono overflow-hidden">
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
              <button className="p-2 flex-1 bg-gray-700">Step</button>
              <button className="p-2 flex-1 bg-gray-700">Run</button>
              <button className="p-2 flex-1 bg-gray-700">Reset</button>
              <button className="p-2 flex-2 bg-gray-700">Assemble</button>
            </div>
            <div className="overflow-auto flex-grow">
              <table className="w-full text-center border-collapse">
                <thead>
                  <tr className="border-b border-gray-700">
                    <th className="p-2">PC</th>
                    <th className="p-2">Machine Code</th>
                    <th className="p-2">Original Code</th>
                  </tr>
                </thead>
                <tbody>
                  {instructions.map((inst, index) => (
                    <tr key={index} className="border-b border-gray-700">
                      <td className="p-2">{inst.pc}</td>
                      <td className="p-2">{inst.machineCode}</td>
                      <td className="p-2">{inst.originalCode}</td>
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
                {registers.map((reg, i) => (
                  <div key={i} className="flex justify-between p-2 border-b border-gray-700 text-lg">
                  <span className="text-center flex-2">{reg.name}</span>
                  <span className="text-center flex-3">{reg.value}</span>
                </div>
                ))}
              </div>
            ) : (
              <div className="overflow-auto max-h-screen border-2 border-gray-700 p-2">
                {Object.entries(memory).map(([addr, value], i) => (
                  <div key={i} className="flex justify-between p-2 border-b border-gray-700">
                    <span>{addr}</span>
                    <span>{value}</span>
                  </div>
                ))}
              </div>
            )}
          </div>
        </div>
      )}
    </div>
      );
}