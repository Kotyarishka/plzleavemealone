const buttons = document.querySelectorAll(".lock-controls .button");

buttons.forEach((button) => {
  button.addEventListener("click", () => {
    const id = button.dataset.id;
    const action = button.dataset.action;

    const url = `/api/v1/lock/${id}/${action}`;

    fetch(url, { method: "POST" })
      .then((res) => res.json())
      .then((data) => {
        console.log(data);
      });

    // reload page
    location.reload();
  });
});
