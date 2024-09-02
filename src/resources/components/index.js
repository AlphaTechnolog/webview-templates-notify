// Style note: Every $ prefixed variable means dom element

// ui error timeout object
let t;

const childFind = ($element, childId) => {
        for (const $child of $element.children) {
                if ($child.id === childId) {
                        return $child;
                }
        }

        return null;
}

const displayUIError = (message) => {
        const $errorEl = document.querySelector("#error-box");
        const $errorLabel = childFind($errorEl, "error-label");

        if ($errorLabel === null) {
                throw new Error("Unable to find #error-label");
        }

        $errorLabel.innerText = message;
        $errorEl.style.display = "block";

        if (t) clearTimeout(t);

        t = setTimeout(() => {
                $errorLabel.innerText = "";
                $errorEl.style.display = "none";
        }, 1000);
}

const catched = exceptable => (...data) => {
        try {
                exceptable(...data);
        } catch (error) {
                window.display(error.toString());
        }
}

const handleNotificationSubmit = e => {
        e.preventDefault();

        const data = new FormData(e.target);
        const message = data.get("message");

        if (message == "") {
                return displayUIError("Type a valid message!");
        }

        window.sendNotification(message);
}

const main = () => {
        const $notificationForm = document.querySelector("#notification-form");

        if (!$notificationForm) {
                throw new Error("Unable to find notification form by id #notification-form");
        }

        $notificationForm.addEventListener("submit", catched(handleNotificationSubmit));
}

const xmain = catched(main);
xmain();
