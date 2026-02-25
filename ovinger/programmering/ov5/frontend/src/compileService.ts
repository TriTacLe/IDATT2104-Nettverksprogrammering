import api from './api'

export const compileService = {
  async compileAndRun(code: string) {
    const res = await api.post('/compile', { code })
    console.log('Response:', res)
    console.log('Response data:', res.data)
    return res.data
  },
}
