const getElem = (id) => document.getElementById(id);

const changedMode = () => {
    let modeElem = getElem("mode");
    if (modeElem.selectedIndex == 0 /* encode mode */) {
        getElem("recordAudio").style.display = "none";
    } else if (modeElem.selectedIndex == 1 /* decode mode */) {
        getElem("recordAudio").style.display = "block";
    }
};

if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
    let constraints = {
        audio: {
          channelCount: 1,
          sampleRate: 44100,
        }
    };
    let recordButton = getElem("audioRecBtn");
    let recordings = getElem("audioRecordings");
    let filePicker = getElem("file");
    let mediaRecorder;
    let mediaStreamTrack;
    let chunks = [];
    
    recordButton.addEventListener("click", () => {
        if (mediaRecorder && mediaRecorder.state === "recording") {
        mediaRecorder.stop();
        if (mediaStreamTrack && mediaStreamTrack.readyState === "live") {
            mediaStreamTrack.stop();
        }
        recordButton.value = "Start Recording";
        } else {
            navigator.mediaDevices.getUserMedia(constraints)
                .then(stream => {
                    mediaRecorder = new MediaRecorder(stream);
                    mediaStreamTrack = stream.getTracks()[0];
                    mediaRecorder.addEventListener("dataavailable", e => {
                        chunks.push(e.data);
                    });
                mediaRecorder.addEventListener("stop", () => {
                    var blob = new Blob(chunks, { type: "audio/wav" });

                    var audioURL = URL.createObjectURL(blob);
                    var downloadLink = document.createElement("a");
                    downloadLink.href = audioURL;
                    downloadLink.download = "recorded.wav";
                    downloadLink.innerHTML = new Date().toLocaleString();
                    recordings.appendChild(downloadLink).appendChild(document.createElement("br"));

                    let recordedFile = new File([blob], 'recorded.wav');
                    let dataTransfer = new DataTransfer();
                    dataTransfer.items.add(recordedFile);
                    filePicker.files = dataTransfer.files;
                    let event = new Event('change', { bubbles: true });
                    filePicker.dispatchEvent(event);

                    chunks = []; // clear for next rec
                });
                mediaRecorder.start();
                recordButton.value = "Stop Recording";
                }).catch(function(err) {
                    console.error("Error accessing the microphone:", err);
                });
        }
})} else {
    console.error("getUserMedia not supported on your browser!");
}

const submit = () => {
    let formData = new FormData();
    formData.append("mode", getElem("mode").selectedIndex);
    formData.append("spf", getElem("spf").value);
    formData.append("file", getElem("file").files[0]);
    
    fetch("/process", {
        method: "POST",
        body: formData
    }).then(res => {
        if (res.ok) {
            const contentDisposition = res.headers.get('Content-Disposition');
            const filenameMatch = contentDisposition && contentDisposition.match(/filename="(.+)"/);
            if (filenameMatch && filenameMatch[1]) {
                const filename = filenameMatch[1];
                return { filename, response: res };
            } else {
                console.error("filename not in response headers");
            }
        } else {
            console.error("error processing file: ", res);
            return Promise.reject();
        }
    }).then(({ filename, response }) => {
        const downloadLink = document.createElement('a');
        return response.blob().then((blob) => {
            const blobUrl = URL.createObjectURL(blob);
            downloadLink.href = blobUrl;
            downloadLink.download = filename;
            downloadLink.click();
            URL.revokeObjectURL(blobUrl);
        })
    }).catch(err => {
        console.error(err);
    });
};