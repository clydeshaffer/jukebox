#ifndef JSONMACROS_H
#define JSONMACROS_H

#define JSON_READ(typ, x) x=obj[#x].Get##typ();
#define JSON_READ_PATH(x) x=path(NormalizePathString(obj[#x].GetString()));
#define JSON_EMIT(typ, x) writer->String(#x);writer->typ(x);
#define JSON_EMIT_STRING(x) writer->String(#x);writer->String(x.c_str());
#define JSON_EMIT_PATH(x) writer->String(#x);writer->String(x.string().c_str());
#endif // JSONMACROS_H
