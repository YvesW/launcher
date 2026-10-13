file(SHA256 "${JAR}" JAR_SHA256)
file(WRITE "${HEADER}" "#pragma once\n#define RUNELITE_JAR_SHA256 \"${JAR_SHA256}\"\n")
