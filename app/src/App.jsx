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

    return (
        <div className="h-screen flex flex-col bg-gray-900 text-green-400 font-mono p-4">
            {/* Pixel Art Title */}
            <div className="text-center mb-4">
                <h1 className="text-2xl font-bold tracking-widest text-green-400 pb-2">
                    RISC-V ASSEMBLER
                </h1>
                <div className="flex justify-center">
                    <div className="w-64 h-1 bg-green-400"></div>
                </div>
            </div>

            {/* Input/Output Windows */}
            <div className="flex flex-grow w-full max-w-7xl mx-auto">
                {/* Left Panel - Assembly Code Input */}
                <div className="w-1/3 flex flex-col mr-4">
                    <div className="bg-gray-800 p-1 mb-1 border-b-4 border-l-4 border-r-4 border-t-4 border-green-500">
                        <h2 className="text-sm font-bold px-2 py-1 bg-green-500 text-gray-900 inline-block">ASSEMBLY</h2>
                    </div>
                    <textarea
                        className="w-full flex-1 p-3 bg-gray-800 text-green-300 border-b-4 border-l-4 border-r-4 border-green-500 outline-none resize-none text-sm"
                        placeholder="Write your assembly code here..."
                        value={assemblyCode}
                        onChange={(e) => setAssemblyCode(e.target.value)}
                        style={{ fontFamily: "monospace", lineHeight: "1.5" }}
                    />
                </div>

                {/* Right Panel - Machine Code Output */}
                <div className="w-2/3 flex flex-col">
                    <div className="bg-gray-800 p-1 mb-1 border-b-4 border-l-4 border-r-4 border-t-4 border-green-500">
                        <h2 className="text-sm font-bold px-2 py-1 bg-green-500 text-gray-900 inline-block">MACHINE CODE</h2>
                    </div>
                    <div
                        className="w-full flex-1 p-3 bg-gray-800 text-green-300 border-b-4 border-l-4 border-r-4 border-green-500 overflow-y-auto"
                        style={{ 
                            fontFamily: "monospace", 
                            lineHeight: "1.5", 
                            whiteSpace: "pre-wrap", 
                            wordBreak: "break-word" 
                        }}
                        dangerouslySetInnerHTML={{ __html: machineCode }}
                    />
                </div>
            </div>

            {/* Action Buttons */}
            <div className="mt-6 flex justify-center gap-6">
                <button
                    className="px-6 py-2 bg-green-500 hover:bg-green-400 text-gray-900 font-bold border-b-4 border-l-2 border-r-2 border-t-2 border-green-700 hover:border-green-600 transition duration-200"
                    onClick={assembleCode}
                    disabled={loading}
                >
                    {loading ? "..." : "ASSEMBLE"}
                </button>
                <button
                    className="px-6 py-2 bg-red-500 hover:bg-red-400 text-gray-900 font-bold border-b-4 border-l-2 border-r-2 border-t-2 border-red-700 hover:border-red-600 transition duration-200"
                    onClick={() => { setAssemblyCode(""); setMachineCode(""); }}
                >
                    CLEAR
                </button>
            </div>

            {/* Error Message */}
            {error && (
                <div className="mt-4 mx-auto max-w-lg p-2 bg-gray-800 border-2 border-red-500">
                    <p className="text-red-500 text-center text-sm font-bold">{error}</p>
                </div>
            )}
        </div>
    );
}