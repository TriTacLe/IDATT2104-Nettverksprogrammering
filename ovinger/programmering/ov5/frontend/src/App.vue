<script setup lang="ts">
import { ref } from 'vue'
import { compileService } from './compileService'

const sourceCode = ref<string>(`#include <iostream>
using namespace std;

int main() {
  cout << "hei" << endl;
  return 0;
}`)

const output = ref('')
const isLoading = ref(false)
function clear() {
  output.value = ''
}

async function fetchRunResult(code: string) {
  try {
    return await compileService.compileAndRun(code)
  } catch (error) {
    console.log('API error: ', error)
    return null
  }
}

async function run() {
  isLoading.value = true
  const timestamp = new Date().toLocaleTimeString()
  const runHeader = `[${timestamp}]`

  const result = await fetchRunResult(sourceCode.value)
  const runBody = result ? `${result}` : 'No run result'
  isLoading.value = false

  output.value = `${runHeader}\n${runBody}\n\n${output.value}`
}
</script>

<template>
  <main class="page">
    <section class="workspace">
      <header class="hero">
        <button class="run-btn" type="button" @click="run">Compile and Run</button>
      </header>

      <div class="panels">
        <section class="panel">
          <label class="panel-title" for="ta"><code>main.cp</code></label>
          <textarea
            id="ta"
            name="main.cpp"
            v-model="sourceCode"
            spellcheck="false"
            placeholder="#include <iostream>
use namespace std;
int main() {
  cout << 'hei' << endl;
  return 0;
}"
          ></textarea>
        </section>

        <section class="panel">
          <div class="output-container">
            <div class="panel-title">Terminal</div>
            <button class="clear-btn" type="button" @click="clear">Clear</button>
          </div>
          <pre class="output">{{ output }}</pre>
          <pre class="output" v-if="isLoading">Compiling</pre>
        </section>
      </div>
    </section>
  </main>
</template>

<style scoped>
.page {
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: clamp(1rem, 2vw, 2rem);
  color: #122033;
  background: #3f5efb;
  background: radial-gradient(circle, rgba(63, 94, 251, 1) 0%, rgba(252, 70, 107, 1) 100%);
}

.workspace {
  width: min(1100px, 100%);
  background: #fff;
  border: 1px solid rgba(18, 32, 51, 0.08);
  border-radius: 20px;
  padding: clamp(1rem, 2vw, 1.5rem);
  box-shadow: 0 12px 30px rgba(18, 32, 51, 0.07);
}

.hero {
  display: flex;
  gap: 2rem;
  flex-direction: column;
  justify-content: space-between;
  gap: 1rem;
  align-items: flex-start;
  margin-bottom: 1rem;
}

.title {
  margin: 0.5rem 0 0;
  color: #0066ff;
  max-width: 55ch;
}

.title code {
  background: #eef3fb;
  border: 1px solid #d9e4f2;
  padding: 0.1rem 0.35rem;
}

.run-btn {
  border: none;
  padding: 0.8rem 1rem;
  font-weight: 700;
  color: #fff;
  background: linear-gradient(135deg, #1d4ed8, #2563eb);
  cursor: pointer;
  white-space: nowrap;
  transition:
    transform 140ms ease,
    box-shadow 140ms ease;
}

.run-btn:hover {
  transform: translateY(-1px);
}

.run-btn:active {
  transform: translateY(0);
}

.panels {
  display: grid;
  grid-template-columns: 1.2fr 1fr;
  gap: 1rem;
}

.panel {
  display: grid;
  grid-template-rows: auto 1fr;
  gap: 0.5rem;
  min-height: 380px;
}

.panel-title {
  font-weight: 700;
  color: #243850;
}

.output-container {
  display: flex;
  flex-direction: row;
  gap: 7rem;
  align-items: space-between;
}

textarea,
.output {
  width: 100%;
  min-height: 0;
  border: 1px solid #d6e0ee;
  background: #fbfdff;
  padding: 0.9rem;
  font:
    500 0.95rem/1.45 ui-monospace,
    SFMono-Regular,
    Menlo,
    Monaco,
    Consolas,
    'Liberation Mono',
    'Courier New',
    monospace;
  color: #13243a;
  box-sizing: border-box;
}

textarea {
  resize: vertical;
  min-height: 380px;
}

textarea:focus-visible {
  outline: 3px solid rgba(0, 255, 17, 0.18);
  border-color: rgb(0, 255, 76);
}

.output {
  margin: 0;
  overflow: auto;
  background: #fbfdff;
  color: #3d4f67;
  white-space: pre-wrap;
}

@media (max-width: 900px) {
  .hero {
    flex-direction: column;
    align-items: stretch;
  }

  .run-btn {
    width: 100%;
  }

  .panels {
    grid-template-columns: 1fr;
  }

  .panel {
    min-height: 0;
  }

  textarea {
    min-height: 280px;
  }
}

@media (prefers-reduced-motion: reduce) {
  .run-btn {
    transition: none;
  }
}
</style>
