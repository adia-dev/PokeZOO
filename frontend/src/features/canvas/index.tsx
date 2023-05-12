import { useEffect, useRef, useState } from 'react'

type Props = {}

const Canvas = (_props: Props) => {

    const canvasRef = useRef<HTMLCanvasElement>(null)
    const [attempts, setAttempts] = useState(0)

    useEffect(() => {
        const canvas = canvasRef.current
        const context = canvas?.getContext('2d')
        if (!canvas || !context) return

        // @ts-ignore
        Module.canvas = canvas

        const attemptIntervalID = setInterval(() => {

            // @ts-ignore
            if (!Module.canvas || !Module.ccall) {
                setAttempts((attempts) => attempts + 1)
                console.log(`Attempt ${attempts} to load Module.canvas and Module.ccall`)
                if (attempts > 10) {
                    clearInterval(attemptIntervalID)
                }
            }
            else {
                clearInterval(attemptIntervalID)
                // @ts-ignore
                Module.ccall('mainf', null, null);
            }
        }, 100)

        function handle_window_resize() {
            if (!canvas) return

            canvas.style.width = '100%'
            canvas.style.height = '100%'
        }

        window.addEventListener('resize', handle_window_resize)

        return () => {
            window.removeEventListener('resize', handle_window_resize)

            // @ts-ignore
            if (Module.canvas && Module.ccall) {
                // @ts-ignore
                Module.ccall('quit', null, null);
            }
        }
    }, [])

    return (
        <div>
            <canvas id="canvas"
                className='absolute top-0 left-0
                transition-all duration-200 ease-in-out delay-300
                w-full h-full
                '
                ref={canvasRef}></canvas>
        </div>
    )
}

export default Canvas