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
            setError("Error assembling code. Please try again.");
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
                    return `<span class="text-gray-500">${parts[0]}</span> <span class="text-green-400">${parts[1]}</span>`;
                }
                return `<span class="text-green-400">${line}</span>`;
            })
            .join("<br>");
    };

    return (
        <div className="h-screen flex flex-col bg-black text-white">
            {/* Title */}
            <h1 className="text-2xl font-semibold text-center py-4 text-gray-300">RISC-V Assembler</h1>

            {/* Input/Output Windows */}
            <div className="flex flex-grow w-full max-w-7xl mx-auto border border-gray-700 rounded-lg overflow-hidden">
                {/* Left Panel - Assembly Code Input */}
                <div className="w-1/3 flex flex-col p-3">
                    <h2 className="text-sm font-medium text-gray-400">Assembly Code</h2>
                    <textarea
                        className="w-full flex-1 p-3 mt-1 bg-gray-900 text-white border border-gray-700 rounded-md outline-none resize-none font-mono text-sm"
                        placeholder="Write your assembly code here..."
                        value={assemblyCode}
                        onChange={(e) => setAssemblyCode(e.target.value)}
                    />
                </div>

                {/* Right Panel - Machine Code Output */}
                <div className="w-2/3 flex flex-col p-3 border-l border-gray-700">
                    <h2 className="text-sm font-medium text-gray-400">Machine Code</h2>
                    <div
                        className="w-full flex-1 p-3 mt-1 bg-gray-900 text-green-400 border border-gray-700 rounded-md font-mono text-sm overflow-y-auto"
                        style={{ whiteSpace: "pre-wrap", wordBreak: "break-word" }}
                        dangerouslySetInnerHTML={{ __html: machineCode }}
                    />
                </div>
            </div>

            {/* Action Buttons */}
            <div className="mt-3 flex justify-center gap-4">
                <button
                    className="px-6 py-2 bg-blue-600 hover:bg-blue-500 text-white rounded-md font-medium text-sm transition duration-200"
                    onClick={assembleCode}
                    disabled={loading}
                >
                    {loading ? "Assembling..." : "Assemble"}
                </button>
                <button
                    className="px-6 py-2 bg-red-600 hover:bg-red-500 text-white rounded-md font-medium text-sm transition duration-200"
                    onClick={() => { setAssemblyCode(""); setMachineCode(""); }}
                >
                    Clear
                </button>
            </div>

            {/* Error Message */}
            {error && <p className="text-red-500 mt-3 text-center text-sm">{error}</p>}
        </div>
    );
}
