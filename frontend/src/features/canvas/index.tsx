import React, { useEffect, useRef, useState } from 'react'

type Props = {}

const Canvas = (props: Props) => {

    const canvasRef = useRef<HTMLCanvasElement>(null)
    const [loadingBar, setLoadingBar] = useState(1000)

    function load() {
        setLoadingBar((loadingBar) => loadingBar + Math.random() * 25)
        if (loadingBar < 100) {
            setTimeout(load, 100)
        }
    }

    useEffect(() => {
        const canvas = canvasRef.current
        const context = canvas?.getContext('2d')
        if (!canvas || !context) return


        const timeoutID = setTimeout(() => {
            canvas.width = window.innerWidth
            canvas.height = window.innerHeight
            Module.canvas = canvasRef.current
            start_function()
            console.log(Module)
        }, 100)

        load()

        function handle_window_resize() {

            if (!canvas) return

            canvas.style.width = '100%'
            canvas.style.height = '100%'
        }

        window.addEventListener('resize', handle_window_resize)

        return () => {
            clearTimeout(timeoutID)
            window.removeEventListener('resize', handle_window_resize)

        }
    }, [])




    return (
        <div>
            {
                loadingBar < 100 &&
                <div className='absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2'>
                    <div className='w-96 h-1 bg-gray-300 rounded-full flex items-center justify-center'>
                        <h1 className='text-center text-white pt-10 absolute font-bold'>
                            {loadingBar.toFixed(0)}%
                        </h1>
                        <div className='h-full bg-blue-500 rounded-full
                            transition-all duration-200 ease-in-out'
                            style={{ width: `${loadingBar}%` }}></div>
                    </div>
                </div>
            }
            <canvas id="canvas"
                className='absolute top-0 left-0
                transition-all duration-200 ease-in-out delay-300
                w-full h-full
                '
                style={{ opacity: loadingBar < 100 ? 0 : 1 }}
                ref={canvasRef}></canvas>
        </div>
    )
}

export default Canvas