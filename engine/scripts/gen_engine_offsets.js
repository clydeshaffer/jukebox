const fs = require('fs');

const mapFileContents = fs.readFileSync(process.argv[2],{encoding:'utf8', flag:'r'});

const sections = mapFileContents.split('\r\n\r\n');

const exportsListByName = sections[2].split(/-+/)[1].trim();

const exportCells = exportsListByName.split(/\s+/);
const engine_exports = [];

for(var i = 0; i < exportCells.length; i+=3) {
    const name = exportCells[i];
    const addr = exportCells[i+1];
    engine_exports.push({
        name, addr
    });
}

const defines = engine_exports.map((ex) => `#define CORESYMBOL_${ex.name} 0x${ex.addr}`).join('\r\n');

console.log('/*core_symbols.h - contains exported memory offsets from JukeboxGT engine*/');
console.log('#ifndef CORE_SYMBOLS_H');
console.log('#define CORE_SYMBOLS_H');
console.log(defines);
console.log('#endif');